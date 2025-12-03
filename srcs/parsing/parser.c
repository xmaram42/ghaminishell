/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 13:34:44 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 13:59:33 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir(int type)
{
	return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
		|| type == TOK_APPEND || type == TOK_HEREDOC);
}

void	add_cmd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

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

static void	start_new_cmd(t_cmd **cmds, t_cmd **current)
{
	*current = new_cmd();
	add_cmd_back(cmds, *current);
}

static void	parser_process_token(t_cmd *current, t_token **tokens, t_data *data)
{
	if ((*tokens)->type == TOK_PIPE)
		start_new_cmd(NULL, &current);
	else if (is_redir((*tokens)->type))
	{
		parse_redir(current, *tokens, data);
		*tokens = (*tokens)->next;
	}
	else
		pars_word(current, *tokens, data);
}

t_cmd	*parser(t_token *tokens, t_data *data)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	start_new_cmd(&cmds, &current);
	data->cmds = cmds;
	while (tokens)
	{
		if (tokens->type == TOK_PIPE)
			start_new_cmd(&cmds, &current);
		else
			parser_process_token(current, &tokens, data);
		if (tokens)
			tokens = tokens->next;
	}
	return (cmds);
}
