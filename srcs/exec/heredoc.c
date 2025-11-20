/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/20 18:16:47 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	set_heredoc_signals(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint;
	sigaction(SIGINT, &sa, old_int);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, old_quit);
}

static void	restore_signals(struct sigaction *old_int,
			struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

static char	*expand_line(char *line, bool expand, char **env_arr, t_data *data)
{
	if (!expand)
		return (ft_strdup(line));
	return (expand_value(line, env_arr, data->exit_code));
}

static bool	read_heredoc_input(int fd, char *delimiter, t_data *data,
		bool expand, char **env_arr)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		errno = 0;
		line = readline("> ");
		if (!line)
		{
			if (errno == EINTR)
			{
				data->exit_code = 130;
				return (false);
			}
			error_type_msg(ERR_HEREDOC_EOF, NULL, delimiter, 0);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_line(line, expand, env_arr, data);
		free(line);
		if (!expanded)
			return (false);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	return (true);
}

int	handle_heredoc(char *delimiter, bool expand, t_data *data)
{
	int			fd;
	char		*filename;
	int			random_num;
	char		*suffix;
	char		**env_arr;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	env_arr = NULL;
	if (expand)
	{
		if (data->env)
			env_arr = lst_to_arr(data->env);
		else
			env_arr = ft_calloc(1, sizeof(char *));
		if (!env_arr)
			return (error_type_msg(ERR_ALLOCATION, NULL, NULL, 0), -1);
	}
	random_num = rand() % 10000;
	suffix = ft_itoa(random_num);
	if (!suffix)
		return (free_array(env_arr), -1);
	filename = ft_strjoin("/tmp/minishell_heredoc_", suffix);
	free(suffix);
	if (!filename)
		return (free_array(env_arr), -1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), free_array(env_arr), -1);
	set_heredoc_signals(&old_int, &old_quit);
	if (!read_heredoc_input(fd, delimiter, data, expand, env_arr))
		return (restore_signals(&old_int, &old_quit), close(fd),
			unlink(filename), free(filename), free_array(env_arr), -1);
	restore_signals(&old_int, &old_quit);
	free_array(env_arr);
	close(fd);
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	return (fd);
}

