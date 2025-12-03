/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:59:27 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 15:19:20 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_for_heredoc(pid_t pid, t_heredoc_args *args)
{
	int	status;

	(void)args;
	waitpid(pid, &status, 0);
	setup_parent_signals();
	return (status);
}

static int	check_heredoc_status(int status, t_heredoc_args *args)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		args->data->exit_code = 130;
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		write(STDOUT_FILENO, "\n", 1);
		args->data->exit_code = 130;
		return (-1);
	}
	return (0);
}

void	heredoc_child(int fd, char *delimiter, t_heredoc_args *args)
{
	int	exit_code;

	signal(SIGINT, SIG_DFL);
	exit_code = 0;
	if (!read_heredoc_input(fd, delimiter, args))
		exit_code = 130;
	close(fd);
	free(args->filename);
	free(args->delimiter);
	free_array(args->env_arr);
	free_token(&args->data->token);
	free_cmds(&args->data->cmds);
	free_env_list(&args->data->env);
	clear_history();
	exit(exit_code);
}

int	run_heredoc_child(char *delimiter, t_heredoc_args *args)
{
	pid_t	pid;
	int		status;

	set_parent_ignore_signals();
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		heredoc_child(args->fd, delimiter, args);
	status = wait_for_heredoc(pid, args);
	return (check_heredoc_status(status, args));
}
