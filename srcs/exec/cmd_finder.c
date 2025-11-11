/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/10 18:30:00 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_path(char *cmd, char **paths)
{
	char	full_path[1024];
	int	i;

	i = 0;
	while (paths[i])
	{
		ft_strlcpy(full_path, paths[i], 1024);
		ft_strlcat(full_path, "/", 1024);
		ft_strlcat(full_path, cmd, 1024);
		if (access(full_path, X_OK) == 0)
			return (ft_strdup(full_path));
		i++;
	}
	return (NULL);
}

static char	*get_paths(t_list *env)
{
	t_list	*tmp;

	if (!env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->str, "PATH=", 5) == 0)
			return (tmp->str + 5);
		tmp = tmp->next;
		if (!tmp || tmp == env)
			break ;
	}
	return (NULL);
}

static char	*cmd_not_found(char *cmd, int path_missing)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (path_missing)
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
	return (NULL);
}

char	*find_cmd(t_data *data, char *cmd)
{
	char	*path_str;
	char	**paths;
	char	*full_path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		ft_putstr_fd("minishell: ", 2);
		perror(cmd);
		return (NULL);
	}
	path_str = get_paths(data->env);
	if (!path_str)
		return (cmd_not_found(cmd, 1));
	paths = ft_split(path_str, ':');
	if (!paths)
		return (NULL);
	full_path = search_in_path(cmd, paths);
	free_array(paths);
	if (full_path)
		return (full_path);
	return (cmd_not_found(cmd, 0));
}