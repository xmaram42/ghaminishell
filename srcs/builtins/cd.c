/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:07:42 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/05 16:36:45 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_env_var(t_list **env, char *key, char *value)
{
	char	*new_var;
	t_list	*tmp;

	if (!env || !key || !value)
		return (1);
	new_var = create_env_var_string(key, value);
	if (!new_var)
		return (1);
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->str, key, ft_strlen(key)) == 0
			&& tmp->str[ft_strlen(key)] == '=')
		{
			free(tmp->str);
			tmp->str = new_var;
			return (0);
		}
		tmp = tmp->next;
		if (tmp == *env)
			break ;
	}
	if (append_to_list(env, new_var))
		return (0);
	return (1);
}

int	handle_cd_with_path(char *path)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	return (0);
}

static char	*get_home_from_env_list(t_list *env)
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

static int	handle_cd_to_home(t_list *env)
{
	char	*home;

	home = get_home_from_env_list(env);
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	return (handle_cd_with_path(home));
}

int	ft_cd(char **args, t_list **env)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];

	if (!env || !*env)
	{
		ft_putstr_fd("minishell: cd: no environment\n", 2);
		return (1);
	}
	if (!getcwd(old_pwd, PATH_MAX))
		return (perror("minishell: cd"), 1);
	if (!args[1])
	{
		if (handle_cd_to_home(*env))
			return (1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	else if (handle_cd_with_path(args[1]))
		return (1);
	if (!getcwd(new_pwd, PATH_MAX))
		return (perror("minishell: cd"), 1);
	if (update_env_var(env, "OLDPWD", old_pwd))
		return (1);
	if (update_env_var(env, "PWD", new_pwd))
		return (1);
	return (0);
}
