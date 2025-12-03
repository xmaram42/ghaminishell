/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:27:30 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 15:28:56 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_stdio(int *stdin_backup, int *stdout_backup)
{
	if (*stdout_backup >= 0)
	{
		dup2(*stdout_backup, 1);
		close(*stdout_backup);
		*stdout_backup = -1;
	}
	if (*stdin_backup >= 0)
	{
		dup2(*stdin_backup, 0);
		close(*stdin_backup);
		*stdin_backup = -1;
	}
}

static void	exec_builtin_part1(char *command_name, t_data *shell,
	t_cmd *command)
{
	if (strings_equal(command_name, "echo"))
		ms_set_exit_status(shell, builtin_echo(command));
	else if (strings_equal(command_name, "cd"))
		ms_set_exit_status(shell, builtin_cd(shell, command));
	else if (strings_equal(command_name, "pwd"))
		ms_set_exit_status(shell, builtin_pwd());
	else if (strings_equal(command_name, "export"))
		ms_set_exit_status(shell, builtin_export(shell, command));
	else if (strings_equal(command_name, "unset"))
		ms_set_exit_status(shell, builtin_unset(shell, command));
}

static void	exec_builtin_part2(char *command_name, t_data *shell,
	t_cmd *command, int *stdio_backup)
{
	if (strings_equal(command_name, "env"))
		ms_set_exit_status(shell, builtin_env(shell));
	else if (strings_equal(command_name, "clear"))
		ms_set_exit_status(shell, clear_builtin());
	else if (strings_equal(command_name, ":"))
		ms_set_exit_status(shell, builtin_colon(command));
	else if (strings_equal(command_name, "exit"))
	{
		restore_stdio(&stdio_backup[0], &stdio_backup[1]);
		ft_exit(shell, command->argv);
	}
}

void	exec_builtin_dispatch(int *stdin_backup, int *stdout_backup,
	t_data *shell, t_cmd *command)
{
	char	*command_name;
	int		stdio_backup[2];

	stdio_backup[0] = *stdin_backup;
	stdio_backup[1] = *stdout_backup;
	command_name = command->argv[0];
	exec_builtin_part1(command_name, shell, command);
	exec_builtin_part2(command_name, shell, command, stdio_backup);
}
