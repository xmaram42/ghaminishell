/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/10/30 16:40:52 by ghsaad           ###   ########.fr       */
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
			ft_putstr_fd("minishell: warning: here-document delimited by EOF\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Expand variables in heredoc if needed
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (true);
}

int	handle_heredoc(char *delimiter, t_data *data)
{
	int		fd;
	char	*filename;
	int		random_num;

	random_num = rand() % 10000;
	filename = ft_strjoin("/tmp/minishell_heredoc_", ft_itoa(random_num));
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