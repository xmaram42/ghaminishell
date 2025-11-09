/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_launcher.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:22:39 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/07 20:35:00 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_stdio(int *stdin_backup, int *stdout_backup)
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

static void	exec_builtin_dispatch(int *stdin_backup, int *stdout_backup,
			t_data *shell, t_cmd *command)
{
	char	*command_name;

	command_name = command->argv[0];
	if (strings_equal(command_name, "echo"))
		shell->exit_code = builtin_echo(command);
	else if (strings_equal(command_name, "cd"))
		shell->exit_code = builtin_cd(shell, command);
	else if (strings_equal(command_name, "pwd"))
		shell->exit_code = builtin_pwd();
	else if (strings_equal(command_name, "export"))
		shell->exit_code = builtin_export(shell, command);
	else if (strings_equal(command_name, "unset"))
		shell->exit_code = builtin_unset(shell, command);
	else if (strings_equal(command_name, "env"))
		shell->exit_code = builtin_env(shell);
	else if (strings_equal(command_name, "clear"))
		shell->exit_code = clear_builtin();
	else if (strings_equal(command_name, "exit"))
	{
		restore_stdio(stdin_backup, stdout_backup);
		ft_exit(shell, command->argv);
	}
}

static bool	prepare_infile(t_cmd *command, int *stdin_backup, t_data *shell)
{
	*stdin_backup = -1;
	if (command->infile < 0)
		return (true);
	*stdin_backup = dup(0);
	if (*stdin_backup < 0 || dup2(command->infile, 0) < 0)
	{
		if (*stdin_backup >= 0)
		{
			close(*stdin_backup);
			*stdin_backup = -1;
		}
		close(command->infile);
		command->infile = -1;
		shell->exit_code = 1;
		return (false);
	}
	close(command->infile);
	command->infile = -1;
	return (true);
}

static bool	prepare_outfile(t_cmd *command, int *stdout_backup, t_data *shell)
{
	if (command->outfile < 0)
		return (true);
	*stdout_backup = dup(1);
	if (*stdout_backup < 0 || dup2(command->outfile, 1) < 0)
	{
		if (*stdout_backup >= 0)
		{
			close(*stdout_backup);
			*stdout_backup = -1;
		}
		close(command->outfile);
		command->outfile = -1;
		shell->exit_code = 1;
		return (false);
	}
	close(command->outfile);
	command->outfile = -1;
	return (true);
}

bool	launch_builtin(t_data *shell, t_cmd *command)
{
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = -1;
	stdout_backup = -1;
	if (!prepare_infile(command, &stdin_backup, shell))
		return (true);
	if (!prepare_outfile(command, &stdout_backup, shell))
	{
		restore_stdio(&stdin_backup, &stdout_backup);
		return (true);
	}
	exec_builtin_dispatch(&stdin_backup, &stdout_backup, shell, command);
	restore_stdio(&stdin_backup, &stdout_backup);
	return (true);
}
