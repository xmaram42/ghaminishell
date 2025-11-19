/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_wrappers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:28:07 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/19 17:37:39 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_data *shell, t_cmd *command)
{
	return (ft_cd(command->argv, &shell->env));
}

int	builtin_pwd(void)
{
	return (ft_pwd());
}

int	builtin_export(t_data *shell, t_cmd *command)
{
	return (ft_export(command->argv, &shell->env));
}

int	builtin_unset(t_data *shell, t_cmd *command)
{
	return (ft_unset(command->argv, &shell->env));
}

int	builtin_env(t_data *shell)
{
	char	**env_arr;

	env_arr = lst_to_arr(shell->env);
	if (!env_arr)
		return (1);
	ft_env(env_arr);
	free_array(env_arr);
	return (0);
}
