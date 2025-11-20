/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:42:14 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 14:30:31 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	store_word(t_syntax_state *state)
{
	if (state->pending_redir)
		state->pending_redir = 0;
	else
		state->has_cmd = 1;
}

int	process_token(t_token *tok, t_data *data, t_syntax_state *state)
{
	if (tok->type == TOK_SEMICOLON)
	{
		syntax_error(data, tok->str);
		return (0);
	}
	if (tok->type == TOK_AND)
	{
		syntax_error(data, tok->str);
		return (0);
	}
	if (tok->type == TOK_PAREN_OPEN || tok->type == TOK_PAREN_CLOSE)
	{
		if (tok->type == TOK_PAREN_OPEN && tok->next
			&& tok->next->type == TOK_PAREN_CLOSE)
			syntax_error(data, tok->next->str);
		else
			syntax_error(data, tok->str);
		return (0);
	}
	if (tok->type == TOK_PIPE)
	{
		if (!handle_pipe(tok, data, state->has_cmd, state->pending_redir))
			return (0);
		state->has_cmd = 0;
		state->pending_redir = 0;
	}
	else if (token_is_redir(tok->type))
	{
		if (!handle_redir(tok, data, state))
			return (0);
	}
	else
		store_word(state);
	return (1);
}

int	validate_tokens(t_token *tokens, t_data *data)
{
	t_syntax_state	state;

	state.has_cmd = 0;
	state.pending_redir = 0;
	while (tokens)
	{
		if (!process_token(tokens, data, &state))
			return (0);
		tokens = tokens->next;
	}
	if (state.pending_redir)
	{
		syntax_error(data, NULL);
		return (0);
	}
	return (1);
}
