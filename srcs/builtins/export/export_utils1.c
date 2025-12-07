/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 23:58:39 by maram             #+#    #+#             */
/*   Updated: 2025/12/05 16:14:10 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_node_match(t_list *node, char *str, int name_len)
{
	if (!ft_strncmp(node->str, str, name_len)
		&& (node->str[name_len] == '\0' || node->str[name_len] == '='))
		return (1);
	return (0);
}

static int	exist(char *str, t_list *env)
{
	int		name_len;
	int		j;
	t_list	*tmp;

	if (!env)
		return (-1);
	name_len = get_var_name_len(str);
	j = 0;
	tmp = env;
	if (check_node_match(tmp, str, name_len))
		return (j);
	tmp = tmp->next;
	j++;
	while (tmp != env)
	{
		if (check_node_match(tmp, str, name_len))
			return (j);
		tmp = tmp->next;
		j++;
	}
	return (-1);
}

static bool	update_existing_var(t_list **env, int pos, char *value)
{
	int		i;
	t_list	*node;

	if (!env || !*env)
	{
		free(value);
		return (false);
	}
	i = 0;
	node = *env;
	while (i++ < pos && node->next)
		node = node->next;
	free((node->str));
	node->str = value;
	return (true);
}

static bool	add_new_var(t_list **env, char *value)
{
	if (!append_to_list(env, value))
	{
		free(value);
		return (false);
	}
	return (true);
}

bool	export(char *str, t_list **env)
{
	int		pos;
	char	*value;

	pos = exist(str, (*env));
	value = ft_strdup(str);
	if (!value)
		return (false);
	if (pos >= 0)
		return (update_existing_var(env, pos, value));
	else if (pos == -1)
		return (add_new_var(env, value));
	return (true);
}
