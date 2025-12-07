/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_launcher.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:22:39 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/05 16:19:39 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ms_set_exit_status(shell, 1);
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

static void	restore_builtin_stdio(int *stdin_backup, int *stdout_backup)
{
	if (*stdout_backup >= 0)
	{
		close(1);
		dup2(*stdout_backup, 1);
		close(*stdout_backup);
		*stdout_backup = -1;
	}
	if (*stdin_backup >= 0)
	{
		close(0);
		dup2(*stdin_backup, 0);
		close(*stdin_backup);
		*stdin_backup = -1;
	}
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
	restore_builtin_stdio(&stdin_backup, &stdout_backup);
	return (true);
}
