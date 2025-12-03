/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 14:00:00 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/03 12:33:35 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_heredoc_file(char **filename, int *write_fd)
{
	*filename = create_heredoc_filename();
	if (!*filename)
		return (-1);
	*write_fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*write_fd < 0)
		return (-1);
	return (0);
}

static void	init_heredoc_args(t_heredoc_args *args, t_hdoc_pack p)
{
	args->expand = p.expand;
	args->env_arr = p.env_arr;
	args->data = p.data;
	args->fd = p.fd;
}

static void	free_heredoc_resources(char *filename, char **env_arr)
{
	free(filename);
	free_array(env_arr);
}

static int	prepare_heredoc_args(
	char *delimiter, bool expand, t_data *data, t_heredoc_args *args)
{
	int				write_fd;
	char			*filename;
	char			**env_arr;
	t_hdoc_pack		p;

	if (setup_heredoc_file(&filename, &write_fd) < 0)
		return (-1);
	env_arr = NULL;
	if (expand)
	{
		env_arr = setup_heredoc_env(expand, data);
		if (!env_arr)
			return (cleanup_heredoc(write_fd, filename, NULL, -1));
	}
	p.expand = expand;
	p.env_arr = env_arr;
	p.data = data;
	p.fd = write_fd;
	init_heredoc_args(args, p);
	args->filename = filename;
	args->delimiter = delimiter;
	return (write_fd);
}

int	handle_heredoc(char *delimiter, bool expand, t_data *data)
{
	int				write_fd;
	int				read_fd;
	t_heredoc_args	args;

	write_fd = prepare_heredoc_args(delimiter, expand, data, &args);
	if (write_fd < 0)
		return (-1);
	if (run_heredoc_child(delimiter, &args) < 0)
		return (cleanup_heredoc(write_fd, args.filename, args.env_arr, -1));
	read_fd = finish_heredoc(write_fd, args.filename);
	free_heredoc_resources(args.filename, args.env_arr);
	if (read_fd < 0)
		return (-1);
	return (read_fd);
}
