/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/24 19:41:18 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_heredoc_line(char *line, bool expand,
			char **env_arr, t_data *data)
{
	if (!expand)
		return (ft_strdup(line));
	return (expand_value(line, env_arr, data->exit_code));
}

static char	**setup_heredoc_env(bool expand, t_data *data)
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

static char	*create_heredoc_filename(void)
{
	int		random_num;
	char	*suffix;
	char	*filename;

	random_num = rand() % 10000;
	suffix = ft_itoa(random_num);
	if (!suffix)
		return (NULL);
	filename = ft_strjoin("/tmp/minishell_heredoc_", suffix);
	free(suffix);
	return (filename);
}

static int	execute_heredoc(char *delimiter, bool expand, t_data *data)
{
	int						fd;
	char					*filename;
	char					**env_arr;
	struct sigaction		sig[2];
	t_heredoc_args			args;

	fd = -1;
	filename = create_heredoc_filename();
	if (!filename)
		return (-1);
	env_arr = setup_heredoc_env(expand, data);
	if (expand && !env_arr)
		return (free(filename), -1);
	args = (t_heredoc_args){expand, env_arr, data};
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), free_array(env_arr), -1);
	set_heredoc_signals(&sig[0], &sig[1]);
	if (!read_heredoc_input(fd, delimiter, &args))
		return (restore_signals(&sig[0], &sig[1]),
			cleanup_heredoc(fd, filename, env_arr, -1));
	restore_signals(&sig[0], &sig[1]);
	return (cleanup_heredoc(fd, filename, env_arr, fd));
}
// this function creates a temp file, reads the heredoc input, writes it to the file,
// and returns the write file descriptor with the filename encoded in the upper bits.

int	handle_heredoc(char *delimiter, bool expand, t_data *data)
{
	int	read_fd;
	int	write_fd;

	write_fd = execute_heredoc(delimiter, expand, data);
	if (write_fd < 0)
		return (-1);
	close(write_fd);
	read_fd = open(((char *[2]){NULL,
				((char *)((long)write_fd & ~0xFF))})[1], O_RDONLY);
	if (read_fd < 0)
		return (-1);
	return (read_fd);
}

/*
in handle heredoc, read fd opens the temp file for reading and returns its fd, the ~0xFF
is just a trick to pass the filename through the int return value, meaning the fd is invalid
 because we used the upper bits to store the filename pointer.
*/