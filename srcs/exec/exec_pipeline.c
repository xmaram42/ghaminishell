/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/25 16:44:19 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_assignment_word(const char *word)
{
	int	i;

	if (!word || !ft_strchr(word, '='))
		return (false);
	if (word[0] != '_' && !ft_isalpha((unsigned char)word[0]))
		return (false);
	i = 0;
	while (word[i] && word[i] != '=')
	{
		if (!ft_isalnum((unsigned char)word[i]) && word[i] != '_')
			return (false);
		i++;
	}
	return (word[i] == '=');
}

static bool	apply_assignment(t_data *data, char *assignment)
{
	if (!export(assignment, &data->env))
	{
		data->exit_code = 1;
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		return (false);
	}
	return (true);
}

static void	shift_arguments(char **argv, int skip)
{
	int	i;

	i = 0;
	while (argv[skip + i])
	{
		argv[i] = argv[skip + i];
		i++;
	}
	argv[i] = NULL;
}

static bool	process_command_assignments(t_data *data, t_cmd *cmd)
{
	int	count;

	if (!cmd->argv)
		return (true);
	count = 0;
	while (cmd->argv[count] && is_assignment_word(cmd->argv[count]))
	{
		if (!apply_assignment(data, cmd->argv[count]))
			return (false);
		free(cmd->argv[count]);
		cmd->argv[count] = NULL;
		count++;
	}
	if (count == 0)
		return (true);
	shift_arguments(cmd->argv, count);
	if (!cmd->argv[0])
	{
		cmd->skip_cmd = true;
		ms_set_exit_status(data, 0);
	}
	return (true);
}

static bool	prepare_assignments(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmds;
	while (cmd)
	{
		if (!process_command_assignments(data, cmd))
			return (false);
		cmd = cmd->next;
	}
	return (true);
}

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
		error_type_msg(ERR_FORK, NULL, NULL, errno);
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
	int		status;
	pid_t	pid;
	int		last_status;
	int		signal_num;

	last_status = 0;
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				signal_num = WTERMSIG(status);
				if (signal_num == SIGQUIT)
					ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
				else if (signal_num == SIGINT)
					ft_putstr_fd("\n", STDOUT_FILENO);
				// only here we do 128 + signal
				last_status = 128 + signal_num;
			}
		}
	}
	ms_set_exit_status(data, last_status);
}

static bool	exec_iteration(t_data *data, t_cmd *cmd, pid_t *child_pid)
{
	int	pip[2];

	pip[0] = -1;
	pip[1] = -1;
	if (cmd->next && pipe(pip) == -1)
	{
		error_type_msg(ERR_PIPE, NULL, NULL, errno);
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
	if (!prepare_assignments(data))
		return (false);
	if (!data->cmds->next && (!data->cmds->argv || !data->cmds->argv[0]))
		return (true);
	if (!data->cmds->next && data->cmds->argv
		&& data->cmds->argv[0] && is_builtin(data->cmds->argv[0]))
		return (launch_builtin(data, data->cmds));
	cmd = data->cmds;
	last_pid = -1;
	set_parent_ignore_signals();
	while (cmd)
	{
		if (!exec_iteration(data, cmd, &child_pid))
		{
			setup_parent_signals();
			return (false);
		}
		if (!cmd->next)
			last_pid = child_pid;
		cmd = cmd->next;
	}
	wait_all(data, last_pid);
	setup_parent_signals();
	return (true);
}
