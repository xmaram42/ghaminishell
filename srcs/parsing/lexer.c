/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:09:37 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 15:08:33 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_parenthesis(t_token **head, char *line, int i)
{
	char	token[2];
	int		type;

	token[0] = line[i];
	token[1] = '\0';
	if (line[i] == '(')
		type = TOK_PAREN_OPEN;
	else
		type = TOK_PAREN_CLOSE;
	add_token_back(head, new_token(ft_strdup(token), type));
	return (i + 1);
}

static int	handle_redir_out(t_token **head, char *line, int i)
{
	if (line[i + 1] == '>')
	{
		add_token_back(head, new_token(ft_strdup(">>"), TOK_APPEND));
		return (i + 2);
	}
	add_token_back(head, new_token(ft_strdup(">"), TOK_REDIR_OUT));
	return (i + 1);
}

// int	handle_word_with_error(t_token **head, char *line, int i)
// {
// 	i = handle_word(head, line, i);
// 	if (i == -1)
// 	{
// 		free_token(head);
// 		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
// 		return (-1);
// 	}
// 	return (i);
// }

int	handle_word_with_error(t_token **head, char *line, int i)
{
	i = handle_word(head, line, i);
	if (i == -1)
	{
		free_token(head);
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		return (-1);
	}
	return (i);
}

t_token	*lexer(char *line)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else if (line[i] == '|')
			i = handle_pipe_token(&head, line, i);
		else if (line[i] == '&')
			i = handle_and_token(&head, line, i);
		else if (line[i] == '<')
			i = handle_redir_in(&head, line, i);
		else if (line[i] == '>')
			i = handle_redir_out(&head, line, i);
		else if (line[i] == '(' || line[i] == ')')
			i = handle_parenthesis(&head, line, i);
		else if (line[i] == ';')
		{
			add_token_back(&head,
				new_token(ft_strdup(";"), TOK_SEMICOLON));
			i++;
		}
		else
		{
			i = handle_word_with_error(&head, line, i);
			if (i == -1)
				return (NULL);
		}
	}
	return (head);
}
