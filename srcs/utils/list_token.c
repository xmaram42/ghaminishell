/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:36:17 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/04 14:24:37 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  token_new_element(t_token **new, char *str, int type)
{
    if (!str)
        return (0);
    (*new) = malloc(sizeof(t_token));
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
/*the function above puts shows how a new element is tokenized, allocating
a malloced string and the type of token it is, ensuring theres nothing
before or after it*/

static void add_first(t_token **list, t_token *new)
{
    (*list) = new;
    (*list)->prev = *list;
    (*list)->next = *list;
}
//When list is empty, create a circular doubly linked list with the
//new node pointing to itself as both next and prev.


int append_token(t_token **list, char *str, int type)
{
    t_token *new;

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
/*Add a new token to the list. If empty → call add_first.
   Else → insert new token at the end (before head), fixing next/prev links.
   Returns 1 on success. */
 
   void	free_token(t_token **list)
   {
	   t_token	*cur;
	   t_token	*first;
	   t_token	*next;
   
	   if (!list || !*list)
		   return ;
	   cur = *list;
	   first = *list;
	   while (cur)
	   {
		   next = cur->next;
		   free(cur->str);
		   free(cur);
		   cur = next;
		   if (cur == first)
			   break ;
	   }
	   *list = NULL;
   }


//Free the entire circular token list, including each token string.
//Resets list pointer to NULL to avoid dangling reference. 

// void    free_everything(t_token **list, char **args)
// {
    
// }