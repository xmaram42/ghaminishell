/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:50:15 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 00:17:52 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	matches_var_name(char *str, char *name, int name_len)
{
	if (ft_strncmp(str, name, name_len) == 0
		&& (str[name_len] == '=' || str[name_len] == '\0'))
		return (1);
	return (0);
}

static void	remove_node(t_list **env, t_list *current, t_list *prev)
{
	if (prev)
		prev->next = current->next;
	else
		*env = current->next;
	free(current->str);
	free(current);
}

static void	remove_env_node(t_list **env, char *name)
{
	t_list	*current;
	t_list	*prev;
	int		name_len;

	if (!env || !*env || !name)
		return ;
	name_len = ft_strlen(name);
	current = *env;
	prev = NULL;
	while (1)
	{
		if (matches_var_name(current->str, name, name_len))
		{
			remove_node(env, current, prev);
			return ;
		}
		prev = current;
		current = current->next;
		if (current == *env)
			break ;
	}
}

int	ft_unset(char **argv, t_list **env)
{
	int	i;

	if (!argv[1])
		return (0);
	i = 1;
	while (argv[i])
	{
		if (is_valid_env_var_name(argv[i]))
			remove_env_node(env, argv[i]);
		else
			error_type_msg(ERR_INVALID_IDENTIFIER,
				"unset", argv[i], 0);
		i++;
	}
	return (0);
}
