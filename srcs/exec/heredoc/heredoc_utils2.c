/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:00:28 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 15:01:42 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	process_heredoc_line(int fd, char *line, t_heredoc_args *args)
{
	char	*expanded;

	expanded = expand_heredoc_line(line, args->expand,
			args->env_arr, args->data);
	free(line);
	if (!expanded)
		return (false);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (true);
}

static bool	handle_heredoc_line(char *line, char *delimiter, int fd,
	t_heredoc_args *args)
{
	if (!line)
	{
		if (errno == EINTR)
			return (args->data->exit_code = 130, false);
		error_type_msg(ERR_HEREDOC_EOF, NULL, delimiter, 0);
		return (false);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (false);
	}
	if (!process_heredoc_line(fd, line, args))
		return (false);
	return (true);
}

bool	read_heredoc_input(int fd, char *delimiter, t_heredoc_args *args)
{
	char	*line;
	bool	result;
	bool	is_delimiter;

	while (true)
	{
		errno = 0;
		line = readline("> ");
		if (!line)
		{
			result = handle_heredoc_line(line, delimiter, fd, args);
			break ;
		}
		is_delimiter = (ft_strcmp(line, delimiter) == 0);
		result = handle_heredoc_line(line, delimiter, fd, args);
		if (is_delimiter)
			break ;
		if (!result)
			return (false);
	}
	return (true);
}

int	cleanup_heredoc(int fd, char *filename, char **env_arr, int return_val)
{
	if (filename)
	{
		if (fd >= 0)
			close(fd);
		unlink(filename);
		free(filename);
	}
	free_array(env_arr);
	return (return_val);
}
