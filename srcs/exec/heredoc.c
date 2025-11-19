/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/18 16:10:44 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	read_heredoc_input(int fd, char *delimiter, t_data *data)
{
	char	*line;

	(void)data;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
				error_type_msg(ERR_HEREDOC_EOF, NULL, delimiter, 0);
				break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (true);
}

int	handle_heredoc(char *delimiter, t_data *data)
{
	int	fd;
	char	*filename;
	int	random_num;
	char	*suffix;

	random_num = rand() % 10000;
	suffix = ft_itoa(random_num);
	if (!suffix)
		return (-1);
	filename = ft_strjoin("/tmp/minishell_heredoc_", suffix);
	free(suffix);
	if (!filename)
		return (-1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(filename);
		return (-1);
	}
	if (!read_heredoc_input(fd, delimiter, data))
	{
		close(fd);
		unlink(filename);
		free(filename);
		return (-1);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	return (fd);
}
