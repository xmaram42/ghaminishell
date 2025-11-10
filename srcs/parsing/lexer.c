/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:09:37 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/10 14:27:21 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//? 
// ** Main lexer: scan the input line and build a list of tokens.
// ** - Skip spaces/tabs.
// ** - Detect operators: |, <, >, <<, >>.
// ** - Otherwise collect a word (command/argument).
// ** Returns head of token linked list.
//?

/* handle operators */
static int handle_pipe(t_token **head, char *line, int i)
{
        if (line[i + 1] == '|')
        {
                add_token_back(head, new_token(ft_strdup("||"), TOK_PIPE));
                return (i + 2);
        }
        add_token_back(head, new_token(ft_strdup("|"), TOK_PIPE));
        return (i + 1);
}

static int handle_redir_in(t_token **head, char *line, int i)
{
    if (line[i + 1] == '<')
    {
        add_token_back(head, new_token(ft_strdup("<<"), TOK_HEREDOC));
        return (i + 2);
    }
    add_token_back(head, new_token(ft_strdup("<"), TOK_REDIR_IN));
    return (i + 1);
}
static int handle_redir_out(t_token **head, char *line, int i)
{
    if (line[i + 1] == '>')
    {
        add_token_back(head, new_token(ft_strdup(">>"), TOK_APPEND));
        return (i + 2);
    }
    add_token_back(head, new_token(ft_strdup(">"), TOK_REDIR_OUT));
    return (i + 1);
}
int	handle_word_with_error(t_token **head, char *line, int i)
{
	i = handle_word(head, line, i);
	if (i == -1)
	{
		free_token(head);
		ft_putstr_fd("minishell: memory allocation failed\n", 2);
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
			i = handle_pipe(&head, line, i);
		else if (line[i] == '<')
			i = handle_redir_in(&head, line, i);
		else if (line[i] == '>')
			i = handle_redir_out(&head, line, i);
		else
		{
			i = handle_word_with_error(&head, line, i);
			if (i == -1)
				return (NULL);
		}
	}
	return (head);
}

//? The lexer = “sentence cutter”.

//? It breaks a raw input string into meaningful pieces (tokens).

//? Those tokens form a linked list.

//?Parser will later look at this list and decide:

//? Which word is the command.

//? Which are arguments.

//? Which are redirections.

//? Where pipes split commands 

/**/
// void    print_tokens(t_token *lst)
// {
//     while (lst)
//     {
//         printf("[type=%d] \"%s\"\n", lst->type, lst->str);
//         lst = lst->next;
//     }
// }
