/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:54:32 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 14:51:42 by maram            ###   ########.fr       */
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

bool	exec_cmd(t_data *data, t_cmd *cmd, int *pip, pid_t *out_pid)
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

static void	handle_signal_status(int signal_num, int *last_status)
{
	if (signal_num == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	else if (signal_num == SIGINT)
		ft_putstr_fd("\n", STDOUT_FILENO);
	*last_status = 128 + signal_num;
}

static void	process_child_status(pid_t pid, pid_t last_pid, int status,
	int *last_status)
{
	int	signal_num;

	if (pid != last_pid)
		return ;
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signal_num = WTERMSIG(status);
		handle_signal_status(signal_num, last_status);
	}
}

void	wait_all(t_data *data, pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		last_status;

	last_status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		process_child_status(pid, last_pid, status, &last_status);
		pid = waitpid(-1, &status, 0);
	}
	ms_set_exit_status(data, last_status);
}
