/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:24:13 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 15:19:04 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_heredoc_filename(void)
{
	static int	counter = 0;
	char		*suffix;
	char		*filename;

	suffix = ft_itoa(counter);
	if (!suffix)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", suffix);
	free(suffix);
	if (!filename)
		return (NULL);
	counter++;
	return (filename);
}

int	finish_heredoc(int fd, char *filename)
{
	int	read_fd;

	if (fd >= 0)
		close(fd);
	read_fd = open(filename, O_RDONLY);
	if (read_fd < 0)
		return (-1);
	unlink(filename);
	return (read_fd);
}

char	*expand_heredoc_line(char *line, bool expand,
	char **env_arr, t_data *data)
{
	if (!expand)
		return (ft_strdup(line));
	return (expand_value(line, env_arr, data->exit_code));
}

char	**setup_heredoc_env(bool expand, t_data *data)
{
	char	**env_arr;

	if (!expand)
		return (NULL);
	if (data->env)
		env_arr = lst_to_arr(data->env);
	else
		env_arr = ft_calloc(1, sizeof(char *));
	if (!env_arr)
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
	return (env_arr);
}
