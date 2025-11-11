/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:06:51 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/11 19:51:24 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*ms_new_env_node(char *s)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->str = s;
	new->next = new;
	new->prev = new;
	return (new);
}

static void	ms_add_first_env(t_list **list, t_list *new)
{
	*list = new;
	(*list)->next = *list;
	(*list)->prev = *list;
}

int	append_to_list(t_list **list, char *s)
{
	t_list	*new;

	new = ms_new_env_node(s);
	if (new == NULL)
		return (0);
	if (*list == NULL)
	{
		ms_add_first_env(list, new);
		return (1);
	}
	new->prev = (*list)->prev;
	new->next = *list;
	(*list)->prev->next = new;
	(*list)->prev = new;
	return (1);
}

int	list_length(t_list *list)
{
	t_list	*head;
	int		len;

	if (list == NULL)
		return (0);
	head = list;
	len = 0;
	while (1)
	{
		len = len + 1;
		list = list->next;
		if (list == NULL || list == head)
			break ;
	}
	return (len);
}

void	free_env_list(t_list **list)
{
	t_list	*head;
	t_list	*cur;
	t_list	*next;

	if (list == NULL || *list == NULL)
		return ;
	head = *list;
	cur = head;
	while (1)
	{
		next = cur->next;
		if (cur->str != NULL)
			free(cur->str);
		free(cur);
		if (next == NULL || next == head)
			break ;
		cur = next;
	}
	*list = NULL;
}
