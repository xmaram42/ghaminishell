/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 12:54:09 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 13:00:54 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_change_directory(char **args, t_list **env, char *old_pwd)
{
	(void)old_pwd;
	if (!args[1])
	{
		if (handle_cd_to_home(*env))
			return (1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		error_type_msg(ERR_OLDPWD_NOT_SET, "cd", NULL, 0);
		return (1);
	}
	else if (handle_cd_with_path(args[1]))
		return (1);
	return (0);
}

int	handle_cd_to_home(t_list *env)
{
	char	*home;

	home = get_home_from_env_list(env);
	if (!home)
	{
		error_type_msg(ERR_HOME_NOT_SET, "cd", NULL, 0);
		return (1);
	}
	return (handle_cd_with_path(home));
}