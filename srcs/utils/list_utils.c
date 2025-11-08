/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:06:51 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/04 14:25:01 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_list **list)
{
	t_list	*current;
	t_list	*first;
	t_list	*next;

	if (!list || !*list)
		return ;
	current = *list;
	first = *list;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
		if (current == first)
			break ;
	}
	*list = NULL;
}
/*
this function frees a circular linked list
by iterating through each node, freeing the string and then the node itself,
then sets the original pointer to NULL
*/

static	int	new_elem_to_string(t_list **new, char *elem)
{
	(*new) = malloc(sizeof(t_list));
	if (*new == NULL)
		return (0);
	(*new)->str = elem;
	(*new)->next = NULL;
	(*new)->prev = NULL;
	return (1);
}
/*
this function creates a new element for the linked lists
by allocating memory for it and initializing its fields
returns 1 on success, 0 on failure
*/

static	void	add_first_elem(t_list **list, t_list *new)
{
	(*list) = new;
	(*list)->next = *list;
	(*list)->prev = *list;
}
/*
this function adds the first element to an empty circular linked list
*/

size_t list_length(t_list *list)
{
	t_list	*temp;
	size_t	count;

	if (list == NULL)
		return(0);
	if ((list))
	{
		temp = list;
		count = 1;
		while (temp->next != list)
		{
			count++;
			temp = temp->next;
		}
		return (count);
	}
	return (0);
}
/*
this function counts the amount of elements
in a circular linked list, the count starts by 1 because 
we start from the head of the list, so its not the same as i = 0;
*/

int	append_to_list(t_list **list, char *elem)
{
	t_list *new;
	
	if (!new_elem_to_string(&new, elem))
		return (0);
	if (*list == NULL)
		add_first_elem(list, new);
	else
	{
		new->prev = (*list)->prev;
		new->next = (*list);
		(*list)->prev->next = new;
		(*list)->prev = new;
	}
	return (1);
}
/*
this function appends an element into a circular linked list
in case there is no element, it calls new_elem_to_string and add_first_elem 
to create the first element and adds it to the list, otherwise it 
adjusts the pointers by moving the new element to the end of the list 
and linking it back to the head to maintain the circular structure
*/