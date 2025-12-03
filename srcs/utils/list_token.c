/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:36:17 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 15:37:15 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	token_new_element(t_token **new, char *str, int type)
{
	if (!str)
		return (0);
	*new = malloc(sizeof(t_token));
	if (*new == NULL)
	{
		free(str);
		return (0);
	}
	(*new)->str = str;
	(*new)->type = type;
	(*new)->next = NULL;
	(*new)->prev = NULL;
	return (1);
}

static void	add_first(t_token **list, t_token *new)
{
	(*list) = new;
	(*list)->prev = *list;
	(*list)->next = *list;
}

int	append_token(t_token **list, char *str, int type)
{
	t_token	*new;

	if (!token_new_element(&new, str, type))
		return (0);
	if (!(*list))
		add_first(list, new);
	else
	{
		new->prev = (*list)->prev;
		new->next = (*list);
		(*list)->prev->next = new;
		(*list)->prev = new;
	}
	return (1);
}

void	free_token(t_token **list)
{
	t_token	*head;
	t_token	*cur;
	t_token	*next;

	if (!list || !*list)
		return ;
	head = *list;
	cur = head;
	while (cur)
	{
		next = cur->next;
		free(cur->str);
		free(cur);
		if (!next || next == head)
			break ;
		cur = next;
	}
	*list = NULL;
}
