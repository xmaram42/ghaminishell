/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:29:29 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 15:35:22 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	strings_equal(const char *a, const char *b)
{
	if (!a || !b)
		return (0);
	return (ft_strcmp(a, b) == 0);
}

int	clear_builtin(void)
{
	write(1, "\033[2J\033[H", 7);
	return (0);
}

int	builtin_echo(t_cmd *command)
{
	return (ft_echo(command->argv));
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "clear")
		|| !ft_strcmp(cmd, ":"))
		return (true);
	return (false);
}

char	*cmd_not_found(char *cmd, int path_missing)
{
	if (path_missing)
		error_type_msg(ERR_NO_SUCH_FILE, cmd, NULL, 0);
	else
		error_type_msg(ERR_CMD_NOT_FOUND, cmd, NULL, 0);
	return (NULL);
}
