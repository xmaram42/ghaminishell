/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:20:19 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/18 16:43:24 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_syntax_state
{
	int	has_cmd;
	int	pending_redir;
}		t_syntax_state;

static void     syntax_error(t_data *data, char *token)
{
	error_type_msg(ERR_SYNTAX, NULL, token, 0);
	data->exit_code = 258;
}

static int	token_is_redir(int type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

static int	is_double_pipe(t_token *tok)
{
	return (tok->str && tok->str[0] == '|' && tok->str[1] == '|');
}

static int	handle_pipe(t_token *tok, t_data *data, int has_cmd,
		int pending_redir)
{
	char	*err;

	if (is_double_pipe(tok))
		err = "||";
	else if (!has_cmd || pending_redir || !tok->next)
		err = "|";
	else if (tok->next->type == TOK_PIPE)
		err = "||";
	else if (tok->next->type != TOK_CMD)
		err = tok->next->str;
	else
		return (1);
	syntax_error(data, err);
	return (0);
}


static int	handle_redir(t_token *tok, t_data *data, t_syntax_state *state)
{
	if (state->pending_redir)
	{
		syntax_error(data, tok->str);
		return (0);
	}
	state->pending_redir = 1;
	return (1);
}

static void	store_word(t_syntax_state *state)
{
	if (state->pending_redir)
		state->pending_redir = 0;
	else
		state->has_cmd = 1;
}
static int	process_token(t_token *tok, t_data *data, t_syntax_state *state)
{
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