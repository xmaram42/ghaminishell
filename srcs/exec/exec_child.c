/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/17 14:23:35 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_cleanup(t_data *data)
{
	if (data->cmds)
		free_cmds(&data->cmds);
	if (data->token)
		free_token(&data->token);
	if (data->env)
		free_env_list(&data->env);
}

static void	child_exit(t_data *data, char *path, char **env, int status)
{
	if (env)
		free_array(env);
	if (path)
		free(path);
	child_cleanup(data);
	exit(status);
}
static bool	report_exec_error(char *path, t_data *data, int code)
{
	ft_putstr_fd("minishell: ", 2);
	perror(path);
	data->exit_code = code;
	return (false);
}

static bool	check_dir(char *path, t_data *data)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (report_exec_error(path, data, 126));
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": is a directory", 2);
		data->exit_code = 126;
		return (false);
	}
	return (true);
}
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

static bool	cmd_exist(char **path, t_data *data, char *cmd)
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

static void	redirect_in_out(t_data *data, t_cmd *cmd, int *pip)
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

void	child_process(t_data *data, t_cmd *cmd, int *pip)
{
	char	*path;
	char	**env;

	path = NULL;
	setup_child_signals();
	redirect_in_out(data, cmd, pip);
	if (cmd->skip_cmd || !cmd->argv || !cmd->argv[0])
	{
		int status;

		status = data->exit_code;
		if (status == 0)
			status = 1;
		child_exit(data, NULL, NULL, status);
	}
	if (is_builtin(cmd->argv[0]))
	{
		launch_builtin(data, cmd);
		child_exit(data, NULL, NULL, data->exit_code);
	}
	if (cmd_exist(&path, data, cmd->argv[0]))
	{
		env = lst_to_arr(data->env);
		if (!env)
			child_exit(data, path, NULL, 1);
		execve(path, cmd->argv, env);
		perror("lolipop🍭: execve");
		child_exit(data, path, env, data->exit_code);
	}
	child_exit(data, path, NULL, data->exit_code);
}

