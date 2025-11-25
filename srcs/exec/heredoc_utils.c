/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:24:13 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/25 16:19:29 by aalbugar         ###   ########.fr       */
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
