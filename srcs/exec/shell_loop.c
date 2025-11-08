/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:13:50 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/04 14:30:36 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int shell_step(t_data *data)
{
    char *line;

    line = read_full_line();

    // 1️⃣ Handle Ctrl+D (EOF)
    if (!line)
    {
        data->exit_flag = true;
        return (0);
    }

    // 2️⃣ Handle just Enter (empty line)
    if (line[0] == '\0')
    {
        free(line);
        line = NULL;
        return (1); // skip parsing and stay in loop
    }

    // 3️⃣ Normal command
    add_history(line);
    if (shell_parse_line(data, line))
        (void)shell_exec(data);
    free(line);
    line = NULL;

    // 4️⃣ Cleanup after each iteration
    free_cmds(&data->cmds);
    free_token(&data->token);

    // 5️⃣ Stop loop if "exit" builtin was triggered
    if (data->exit_flag)
        return (0);

    return (1);
}


void	shell_teardown(t_data *data)
{
	rl_clear_history();

	// Always free in a safe order
	free_cmds(&data->cmds);
	free_token(&data->token);
	free_list(&data->env);

	// Optional: if you have other allocations (pipes, buffers, etc.)
	shell_cleanup(data);
	data->cmds = NULL;
	data->token = NULL;
	data->env = NULL;
}
