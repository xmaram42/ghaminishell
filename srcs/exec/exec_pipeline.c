/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/09 17:29:56 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parent_process(t_data *data, t_cmd *cmd, int *pip)
{
	(void)data;
	if (pip[1] >= 0)
	{
		close(pip[1]);
		pip[1] = -1;
	}
	if (cmd->infile >= 0)
	{
		close(cmd->infile);
		cmd->infile = -1;
	}
	if (cmd->next && pip[0] >= 0 && cmd->next->infile == -1)
	{
		cmd->next->infile = pip[0];
		pip[0] = -1;
	}
	if (pip[0] >= 0)
	{
		close(pip[0]);
		pip[0] = -1;
	}
}

static bool	exec_cmd(t_data *data, t_cmd *cmd, int *pip, pid_t *out_pid)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (false);
	}
	if (pid == 0)
	{
		child_process(data, cmd, pip);
		exit(data->exit_code);
	}
	*out_pid = pid;
	parent_process(data, cmd, pip);
	return (true);
}

static void	wait_all(t_data *data, pid_t last_pid)
{
	int	status;
	pid_t	pid;
	int	last_status;

	last_status = 0;
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	data->exit_code = last_status;
}

static bool	exec_iteration(t_data *data, t_cmd *cmd, pid_t *child_pid)
{
	int	pip[2];

	pip[0] = -1;
	pip[1] = -1;
	if (cmd->next && pipe(pip) == -1)
	{
		perror("minishell: pipe");
		if (cmd->infile >= 0)
		{
			close(cmd->infile);
			cmd->infile = -1;
		}
		return (false);
	}
	if (!exec_cmd(data, cmd, pip, child_pid))
	{
		if (pip[0] >= 0)
			close(pip[0]);
		if (pip[1] >= 0)
			close(pip[1]);
		return (false);
	}
	return (true);
}

bool	exec_pipeline(t_data *data)
{
	t_cmd	*cmd;
	pid_t	child_pid;
	pid_t	last_pid;

	if (!data->cmds)
		return (true);
		if (!data->cmds->next && data->cmds->argv
			&& data->cmds->argv[0] && is_builtin(data->cmds->argv[0]))
		return (launch_builtin(data, data->cmds));
	cmd = data->cmds;
	last_pid = -1;
	while (cmd)
	{
		if (!exec_iteration(data, cmd, &child_pid))
			return (false);
		if (!cmd->next)
			last_pid = child_pid;
		cmd = cmd->next;
	}
	wait_all(data, last_pid);
	return (true);
}
