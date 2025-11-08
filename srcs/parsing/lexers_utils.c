/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:20:32 by aalbugar          #+#    #+#             */
/*   Updated: 2025/09/24 12:48:49 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
** Create a new token node for the lexer.
** str  = string value of the token (malloc'd or duplicated before).
** type = token type (see TOK_* macros).
** next = NULL (set later when added to list).
*/

t_token *new_token(char *str, int type)
{
	t_token *tok;
	
	tok = malloc(sizeof(t_token));
	if(!tok)
		return(NULL);
	tok->str = str;
	tok->type = type;
	tok->next = NULL;
	return(tok);
}
/*Add a token node at the end of the token list.
** lst = pointer to the head of the list.
** new = the new token to insert.
** If list is empty, new becomes the first element.*/

void add_token_back(t_token **lst, t_token *new)
{
	t_token *tmp;

	if(!new)
		return;
	if(!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}