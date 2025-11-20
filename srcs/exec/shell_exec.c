/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:59:49 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/20 14:59:50 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	shell_parse_line(t_data *data, char *line)
{
	t_token	*tokens;

	if (has_unclosed_quote(line))
	{
		error_type_msg(ERR_UNCLOSED_QUOTE, NULL, NULL, 0);
		ms_set_exit_status(data, 2);
		return (false);
	}
	tokens = lexer(line);
	if (!tokens)
		return (false);
	if (!validate_tokens(tokens, data))
	{
		free_token(&tokens);
		return (false);
	}
	data->token = tokens;
	data->cmds = parser(tokens, data);
	if (!data->cmds)
	{
		free_token(&tokens);
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

void	shell_cleanup(t_data *data)
{
	(void)data;
}
