/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:59:49 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 15:36:16 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_quote_errors(t_data *data, char *line)
{
	if (has_unclosed_quote(line))
	{
		error_type_msg(ERR_UNCLOSED_QUOTE, NULL, NULL, 0);
		ms_set_exit_status(data, 2);
		return (false);
	}
	return (true);
}

static bool	tokenize_and_validate(t_data *data, char *line, t_token **tokens)
{
	*tokens = lexer(line);
	if (!*tokens)
		return (false);
	if (!validate_tokens(*tokens, data))
	{
		free_token(tokens);
		return (false);
	}
	return (true);
}

bool	shell_parse_line(t_data *data, char *line)
{
	t_token	*tokens;

	if (!check_quote_errors(data, line))
		return (false);
	if (!tokenize_and_validate(data, line, &tokens))
		return (false);
	data->token = tokens;
	data->cmds = parser(tokens, data);
	if (!data->cmds)
	{
		free_token(&tokens);
		data->token = NULL;
		return (false);
	}
	return (true);
}

bool	shell_exec(t_data *data)
{
	if (!data->cmds)
		return (false);
	return (exec_pipeline(data));
}
