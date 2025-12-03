/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:13:00 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 15:33:02 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ensure_executable_path(char *path, t_data *data)
{
	if (access(path, F_OK) != 0)
		return (report_exec_error(path, data, 127));
	if (!check_dir(path, data))
		return (false);
	if (access(path, X_OK) != 0)
		return (report_exec_error(path, data, 126));
	return (true);
}

static bool	set_cmd_error(t_data *data, int code)
{
	data->exit_code = code;
	return (false);
}

bool	cmd_exist(char **path, t_data *data, char *cmd)
{
	if (!ft_strchr(cmd, '/'))
		*path = find_cmd(data, cmd);
	else
	{
		*path = ft_strdup(cmd);
		if (!*path)
			return (false);
	}
	if (!(*path))
		return (set_cmd_error(data, 127));
	if (!ensure_executable_path(*path, data))
	{
		free(*path);
		*path = NULL;
		return (false);
	}
	return (true);
}

void	redirect_in_out(t_data *data, t_cmd *cmd, int *pip)
{
	(void)data;
	if (pip[0] >= 0)
		close(pip[0]);
	if (cmd->infile >= 0)
	{
		dup2(cmd->infile, 0);
		close(cmd->infile);
		cmd->infile = -1;
	}
	if (cmd->outfile >= 0)
	{
		dup2(cmd->outfile, 1);
		close(cmd->outfile);
		cmd->outfile = -1;
	}
	else if (cmd->next && pip[1] >= 0)
		dup2(pip[1], 1);
	if (pip[1] >= 0)
		close(pip[1]);
}
