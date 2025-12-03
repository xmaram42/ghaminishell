/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:08:52 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 00:13:12 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_export_arg(char *arg, t_list **env)
{
	if (!valid_identifier(arg))
	{
		error_type_msg(ERR_INVALID_IDENTIFIER, "export", arg, 0);
		return (1);
	}
	if (!export(arg, env))
	{
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		return (-1);
	}
	return (0);
}

int	handle_no_args(t_list **env)
{
	if (*env && !export_no_args((*env)))
	{
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		return (1);
	}
	return (0);
}

int	ft_export(char **str, t_list **env)
{
	int	exit_code;
	int	i;
	int	result;

	exit_code = 0;
	i = 1;
	if (!str || !str[i])
		return (handle_no_args(env));
	while (str[i])
	{
		result = process_export_arg(str[i], env);
		if (result == -1)
			return (1);
		if (result == 1)
			exit_code = 1;
		i++;
	}
	return (exit_code);
}
