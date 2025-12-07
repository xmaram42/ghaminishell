/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:12:28 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 00:07:57 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_env_var(t_list *env, char *key)
{
	t_list	*tmp;
	size_t	key_len;

	tmp = env;
	key_len = ft_strlen(key);
	while (tmp)
	{
		if (ft_strncmp(tmp->str, key, key_len) == 0
			&& tmp->str[key_len] == '=')
			return (tmp);
		tmp = tmp->next;
		if (tmp == env)
			break ;
	}
	return (NULL);
}

int	update_env_var(t_list **env, char *key, char *value)
{
	char	*new_var;
	t_list	*node;

	if (!env || !key || !value)
		return (1);
	new_var = create_env_var_string(key, value);
	if (!new_var)
		return (1);
	node = find_env_var(*env, key);
	if (node)
	{
		free(node->str);
		node->str = new_var;
		return (0);
	}
	if (append_to_list(env, new_var))
		return (0);
	return (1);
}

int	cd_change_directory(char **args, t_list **env, char *old_pwd)
{
	(void)old_pwd;
	if (!args[1])
	{
		if (handle_cd_to_home(*env))
			return (1);
	}
	else if (args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
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
