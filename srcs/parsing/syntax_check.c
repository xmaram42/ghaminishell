/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:20:19 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/20 13:57:49 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error(t_data *data, char *token)
{
	error_type_msg(ERR_SYNTAX, NULL, token, 0);
	data->exit_code = 258;
}

int	token_is_redir(int type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

int	is_double_pipe(t_token *tok)
{
	return (tok->str && tok->str[0] == '|' && tok->str[1] == '|');
}

int	handle_pipe(t_token *tok, t_data *data, int has_cmd,
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

int	handle_redir(t_token *tok, t_data *data, t_syntax_state *state)
{
	if (state->pending_redir)
	{
		syntax_error(data, tok->str);
		return (0);
	}
	state->pending_redir = 1;
	return (1);
}
