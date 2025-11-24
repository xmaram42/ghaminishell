/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:24:13 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/24 15:25:54 by ghsaad           ###   ########.fr       */
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

bool	read_heredoc_input(int fd, char *delimiter, t_heredoc_args *args)
{
	char	*line;

	while (true)
	{
		errno = 0;
		line = readline("> ");
		if (!line)
		{
			if (errno == EINTR)
				return (args->data->exit_code = 130, false);
			error_type_msg(ERR_HEREDOC_EOF, NULL, delimiter, 0);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (!process_heredoc_line(fd, line, args))
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
