/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 00:54:05 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	setup_pipe_if_needed(t_cmd *cmd, int *pip)
{
	pip[0] = -1;
	pip[1] = -1;
	if (!cmd->next)
		return (true);
	if (pipe(pip) == -1)
	{
		error_type_msg(ERR_PIPE, NULL, NULL, errno);
		if (cmd->infile >= 0)
		{
			close(cmd->infile);
			cmd->infile = -1;
		}
		return (false);
	}
	return (true);
}

static bool	exec_iteration(t_data *data, t_cmd *cmd, pid_t *child_pid)
{
	int	pip[2];

	if (!setup_pipe_if_needed(cmd, pip))
		return (false);
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

static bool	is_single_builtin(t_data *data)
{
	if (!data->cmds->next && data->cmds->argv
		&& data->cmds->argv[0] && is_builtin(data->cmds->argv[0]))
		return (true);
	return (false);
}

static bool	execute_pipeline_loop(t_data *data, pid_t *last_pid)
{
	t_cmd	*cmd;
	pid_t	child_pid;

	cmd = data->cmds;
	*last_pid = -1;
	set_parent_ignore_signals();
	while (cmd)
	{
		if (!exec_iteration(data, cmd, &child_pid))
		{
			setup_parent_signals();
			return (false);
		}
		if (!cmd->next)
			*last_pid = child_pid;
		cmd = cmd->next;
	}
	return (true);
}

bool	exec_pipeline(t_data *data)
{
	pid_t	last_pid;

	if (!data->cmds)
		return (true);
	if (!prepare_assignments(data))
		return (false);
	if (!data->cmds->next && (!data->cmds->argv || !data->cmds->argv[0]))
		return (true);
	if (is_single_builtin(data))
		return (launch_builtin(data, data->cmds));
	if (!execute_pipeline_loop(data, &last_pid))
		return (false);
	wait_all(data, last_pid);
	setup_parent_signals();
	return (true);
}
