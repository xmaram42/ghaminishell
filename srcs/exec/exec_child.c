/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/06 16:15:37 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_dir(char **path, char *cmd, t_data *data)
{
	struct stat	path_stat;

	stat(*path, &path_stat);
	if (!S_ISREG(path_stat.st_mode))
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		data->exit_code = 126;
		return (false);
	}
	return (true);
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
	{
		data->exit_code = 127;
		return (false);
	}
	if (access((*path), X_OK))
	{
		perror(*path);
		free(*path);
		*path = NULL;
		data->exit_code = 126;
		return (false);
	}
	if (!check_dir(path, cmd, data))
		return (false);
	return (true);
}

static void	redirect_in_out(t_data *data, t_cmd *cmd, int *pip)
{
	(void)data;
	close(pip[0]);
	if (cmd->infile >= 0)
	{
		dup2(cmd->infile, 0);
		close(cmd->infile);
	}
	if (cmd->outfile >= 0)
	{
		dup2(cmd->outfile, 1);
		close(cmd->outfile);
	}
	else if (cmd->next)
		dup2(pip[1], 1);
	close(pip[1]);
}

void    child_process(t_data *data, t_cmd *cmd, int *pip)
{
    char    *path;
    char    **env;

    path = NULL;
    setup_child_signals();

    /* Always configure stdin/stdout first (redir + pipe) */
    redirect_in_out(data, cmd, pip);

    if (cmd->skip_cmd)
        exit(1);

    /* Builtins executed in child must now write to the (possibly dup'd) stdout */
    if (is_builtin(cmd->argv[0]))
    {
        launch_builtin(data, cmd);
        exit(data->exit_code);
    }
    /* External command */
    if (cmd_exist(&path, data, cmd->argv[0]))
    {
        env = lst_to_arr(data->env);
        if (!env)
            exit(1);
        execve(path, cmd->argv, env);
        perror("minishell: execve");
        free_array(env);
    }
    if (path)
        free(path);
    exit(data->exit_code);
}
