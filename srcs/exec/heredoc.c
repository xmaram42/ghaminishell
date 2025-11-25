/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 14:00:00 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/25 14:32:34 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child(int fd, char *delimiter, t_heredoc_args *args)
{
	signal(SIGINT, SIG_DFL);
	if (!read_heredoc_input(fd, delimiter, args))
		exit(130);
	exit(0);
}

static int	run_heredoc_child(char *delimiter, t_heredoc_args *args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		heredoc_child(args->fd, delimiter, args);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		args->data->exit_code = 130;
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		args->data->exit_code = 130;
		return (-1);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, bool expand, t_data *data)
{
	int				write_fd;
	int				read_fd;
	char			*filename;
	char			**env_arr;
	t_heredoc_args	args;

	filename = create_heredoc_filename();
	if (!filename)
		return (-1);
	write_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (write_fd < 0)
		return (-1);
	env_arr = NULL;
	if (expand)
	{
		env_arr = setup_heredoc_env(expand, data);
		if (!env_arr)
			return (cleanup_heredoc(write_fd, filename, NULL, -1));
	}
	args.expand = expand;
	args.env_arr = env_arr;
	args.data = data;
	args.fd = write_fd;
	if (run_heredoc_child(delimiter, &args) < 0)
		return (cleanup_heredoc(write_fd, filename, env_arr, -1));
	read_fd = finish_heredoc(write_fd, filename);
	free(filename);
	free_array(env_arr);
	if (read_fd < 0)
		return (-1);
	return (read_fd);
}
