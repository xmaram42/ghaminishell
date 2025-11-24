/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:20:46 by codespace         #+#    #+#             */
/*   Updated: 2025/11/24 17:51:25 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signals2(void)
{
	signal(SIGQUIT, SIG_DFL);
}

void	heredoc_sigint(int signo)
{
	if (signo == SIGINT)
		g_signal_pid = signo;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	set_heredoc_signals(struct sigaction *old_int,
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

void	restore_signals(struct sigaction *old_int,
				struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}