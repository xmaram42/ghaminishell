/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:59:49 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/25 14:56:15 by aalbugar         ###   ########.fr       */
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
	// t_token *tmp = tokens;
	// while(tmp)
	// {
	// 	printf("Token: %s, Type: %d\n", tmp->str, tmp->type);
	// 	tmp = tmp->next;
	// }
	if (!tokens)
		return (false);
	if (!validate_tokens(tokens, data))
	{
		free_token(&tokens);
		return (false);
	}
	data->token = tokens;
	data->cmds = parser(tokens, data);
	// printf("PARSER DONE\n");
	// t_cmd *tmp_cmd = data->cmds;
	// while (tmp_cmd)
	// {
	// 	printf("Command:\n");
	// 	for (int i = 0; tmp_cmd->argv && tmp_cmd->argv[i]; i++)
	// 		printf("  Arg[%d]: %s\n", i, tmp_cmd->argv[i]);
	// 	printf("  Infile: %d\n", tmp_cmd->infile);
	// 	printf("  Outfile: %d\n", tmp_cmd->outfile);
	// 	printf("Skip Command: %s\n", tmp_cmd->skip_cmd ? "true" : "false");
	// 	tmp_cmd = tmp_cmd->next;
	// }
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
