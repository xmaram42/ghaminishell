/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 15:32:16 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_skip_or_empty(t_data *data, t_cmd *cmd)
{
	int	status;

	if (cmd->skip_cmd || !cmd->argv || !cmd->argv[0])
	{
		status = data->exit_code;
		if (status == 0)
			status = 1;
		child_exit(data, NULL, NULL, status);
	}
}

static void	execute_external(t_data *data, t_cmd *cmd, char *path)
{
	char	**env;

	env = lst_to_arr(data->env);
	if (!env)
	{
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		child_exit(data, path, NULL, 1);
	}
	execve(path, cmd->argv, env);
	error_type_msg(ERR_EXECVE, NULL, path, errno);
	child_exit(data, path, env, data->exit_code);
}

void	child_process(t_data *data, t_cmd *cmd, int *pip)
{
	char	*path;

	path = NULL;
	setup_child_signals();
	redirect_in_out(data, cmd, pip);
	handle_skip_or_empty(data, cmd);
	if (handle_single_dot(data, cmd->argv))
		child_exit(data, NULL, NULL, data->exit_code);
	if (is_builtin(cmd->argv[0]))
	{
		launch_builtin(data, cmd);
		child_exit(data, NULL, NULL, data->exit_code);
	}
	if (cmd_exist(&path, data, cmd->argv[0]))
		execute_external(data, cmd, path);
	child_exit(data, path, NULL, data->exit_code);
}
