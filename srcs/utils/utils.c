/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:46:20 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/07 16:58:56 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	disable_echoctl(void)
{
	struct termios	t;

	if (tcgetattr(STDIN_FILENO, &t) == -1)
		return ;
	t.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ms_set_exit_status(t_data *data, int status)
{
	data->exit_code = status;
}

int	ms_get_exit_status(t_data *data)
{
	return (data->exit_code);
}

void	init_data(t_data *data, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	data->env = NULL;
	data->cmds = NULL;
	data->token = NULL;
	data->exit_code = 0;
	data->pip[0] = -1;
	data->pip[1] = -1;
	data->sq = false;
	data->exit_flag = false;
	g_signal_pid = 0;
}
