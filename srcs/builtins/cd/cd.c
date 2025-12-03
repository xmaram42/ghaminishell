/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:07:42 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 00:23:54 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cd_with_path(char *path)
{
	if (chdir(path) == -1)
	{
		error_type_msg(ERR_CHDIR, "cd", path, errno);
		return (1);
	}
	return (0);
}

char	*get_home_from_env_list(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->str, "HOME=", 5) == 0)
			return (tmp->str + 5);
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (NULL);
}

static int	get_current_dir(char *pwd)
{
	if (!getcwd(pwd, PATH_MAX))
	{
		error_type_msg(ERR_GENERAL, "cd", "getcwd", errno);
		return (1);
	}
	return (0);
}

static int	update_pwd_vars(t_list **env, char *old_pwd, char *new_pwd)
{
	if (update_env_var(env, "OLDPWD", old_pwd))
		return (1);
	if (update_env_var(env, "PWD", new_pwd))
		return (1);
	return (0);
}

int	ft_cd(char **args, t_list **env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!env || !*env)
	{
		error_type_msg(ERR_ENV_NOT_SET, "cd", NULL, 0);
		return (1);
	}
	if (get_current_dir(old_pwd))
		return (1);
	if (cd_change_directory(args, env, old_pwd))
		return (1);
	if (get_current_dir(new_pwd))
		return (1);
	return (update_pwd_vars(env, old_pwd, new_pwd));
}
