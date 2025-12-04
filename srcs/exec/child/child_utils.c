/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:11:19 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 15:34:39 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_cleanup(t_data *data)
{
	if (data->cmds)
		free_cmds(&data->cmds);
	if (data->token)
		free_token(&data->token);
	// if (data->env)
	// 	free_env_list(&data->env);
}

bool	handle_single_dot(t_data *data, char **argv)
{
	if (!argv || !argv[0] || ft_strcmp(argv[0], ".") != 0)
		return (false);
	ft_putstr_fd(".: filename argument required\n", STDERR_FILENO);
	ft_putstr_fd(".: usage: . filename [arguments]\n", STDERR_FILENO);
	data->exit_code = 2;
	return (true);
}

void	child_exit(t_data *data, char *path, char **env, int status)
{
	if (env)
		free_array(env);
	if (path)
		free(path);
	child_cleanup(data);
	exit(status);
}

bool	report_exec_error(char *path, t_data *data, int code)
{
	error_type_msg(ERR_GENERAL, NULL, path, errno);
	data->exit_code = code;
	return (false);
}

bool	check_dir(char *path, t_data *data)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (report_exec_error(path, data, 126));
	if (S_ISDIR(path_stat.st_mode))
	{
		error_type_msg(ERR_IS_DIRECTORY, path, NULL, 0);
		data->exit_code = 126;
		return (false);
	}
	return (true);
}
