/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:13:50 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/05 16:21:43 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_clear_step(t_data *data)
{
	if (data->cmds != NULL)
		free_cmds(&data->cmds);
	if (data->token != NULL)
		free_token(&data->token);
}

static int	handle_empty_or_signal(char *line, t_data *data)
{
	if (line == NULL)
	{
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			return (1);
		}
		ft_putendl_fd("exit", 1);
		data->exit_flag = 1;
		return (0);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (1);
	}
	return (-1);
}

int	shell_step(t_data *data)
{
	char	*line;
	int		status;

	line = read_full_line();
	status = handle_empty_or_signal(line, data);
	if (status != -1)
		return (status);
	add_history(line);
	if (shell_parse_line(data, line) != 0)
		(void)shell_exec(data);
	free(line);
	ms_clear_step(data);
	if (data->exit_flag == 1)
		return (0);
	return (1);
}

void	shell_teardown(t_data *data)
{
	clear_history();
	free_cmds(&data->cmds);
	free_token(&data->token);
	if (data->env != NULL)
		free_env_list(&data->env);
}
