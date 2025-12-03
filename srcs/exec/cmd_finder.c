/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 15:26:42 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_path(char *cmd, char **paths)
{
	char	full_path[1024];
	int		i;

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

static char	*handle_empty_or_absolute(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
	{
		if (cmd)
			return (cmd_not_found(cmd, 0));
		else
			return (cmd_not_found("", 0));
	}
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*search_cmd_in_paths(t_data *data, char *cmd)
{
	char	*path_str;
	char	**paths;
	char	*full_path;

	path_str = get_paths(data->env);
	if (!path_str || *path_str == '\0')
		return (cmd_not_found(cmd, 1));
	paths = ft_split(path_str, ':');
	if (!paths)
		return (NULL);
	full_path = search_in_path(cmd, paths);
	free_array(paths);
	return (full_path);
}

char	*find_cmd(t_data *data, char *cmd)
{
	char	*result;
	char	*full_path;

	result = handle_empty_or_absolute(cmd);
	if (result)
		return (result);
	full_path = search_cmd_in_paths(data, cmd);
	if (full_path)
		return (full_path);
	return (cmd_not_found(cmd, 0));
}
