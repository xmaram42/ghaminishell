/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:13:50 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/12 14:03:26 by ghsaad           ###   ########.fr       */
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
int	shell_step(t_data *data)
{
	char	*line;

	line = read_full_line();
	if (line == NULL)
	{
		data->exit_flag = 1;
		return (0);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (1);
	}
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
	rl_clear_history();
	free_cmds(&data->cmds);
	free_token(&data->token);
	if (data->env != NULL)
		free_env_list(&data->env);
}
