/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:50:15 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/17 14:05:29 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

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
		if (ft_strncmp(current->str, name, name_len) == 0
			&& (current->str[name_len] == '=' || current->str[name_len] == '\0'))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->str);
			free(current);
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
		{
			ft_putstr_fd("lolipop🍭: unset: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		i++;
	}
	return (0);
}