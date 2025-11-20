/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:20:32 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 15:11:25 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *str, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->str = str;
	tok->type = type;
	tok->next = NULL;
	return (tok);
}

void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

bool	is_plain_break(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == ';'
		|| c == '&')
		return (true);
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

bool	is_word_stop(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == ';'
		|| c == '&')
		return (true);
	return (false);
}
