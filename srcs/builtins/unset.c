/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:50:15 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/05 16:15:42 by aalbugar         ###   ########.fr       */
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

static void	remove_node(t_list **env, t_list *current)
{
	if (current->next == current)
	{
		*env = NULL;
		free(current->str);
		free(current);
		return ;
	}
	current->next->prev = current->prev;
	current->prev->next = current->next;
	if (current == *env)
		*env = current->next;
	free(current->str);
	free(current);
}

static void	remove_env_node(t_list **env, char *name)
{
	t_list	*current;
	int		name_len;

	if (!env || !*env || !name)
		return ;
	name_len = ft_strlen(name);
	current = *env;
	while (1)
	{
		if (matches_var_name(current->str, name, name_len))
		{
			remove_node(env, current);
			return ;
		}
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
