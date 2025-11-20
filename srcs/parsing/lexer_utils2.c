/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:24:35 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/20 15:10:04 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_pipe_token(t_token **head, char *line, int i)
{
	if (line[i + 1] == '|')
	{
		add_token_back(head, new_token(ft_strdup("||"), TOK_PIPE));
		return (i + 2);
	}
	add_token_back(head, new_token(ft_strdup("|"), TOK_PIPE));
	return (i + 1);
}

int	handle_and_token(t_token **head, char *line, int i)
{
	if (line[i + 1] == '&')
	{
		add_token_back(head, new_token(ft_strdup("&&"), TOK_AND));
		return (i + 2);
	}
	add_token_back(head, new_token(ft_strdup("&"), TOK_AND));
	return (i + 1);
}

int	handle_redir_in(t_token **head, char *line, int i)
{
	if (line[i + 1] == '<')
	{
		add_token_back(head, new_token(ft_strdup("<<"), TOK_HEREDOC));
		return (i + 2);
	}
	add_token_back(head, new_token(ft_strdup("<"), TOK_REDIR_IN));
	return (i + 1);
}

int	get_quoted_str(char *line, int index, char **out)
{
	char	quote;
	int		start;

	quote = line[index];
	index = index + 1;
	start = index;
	while (line[index] && line[index] != quote)
		index = index + 1;
	if (line[index] == '\0')
		return (-1);
	*out = ft_substr(line, start, index - start);
	if (!*out)
		return (-1);
	if (quote == '\'')
		*out = wrap_with_marker(*out, SQ_MARKER);
	else
		*out = wrap_with_marker(*out, DQ_MARKER);
	if (!*out)
		return (-1);
	return (index + 1);
}

int	has_unclosed_quote(const char *line)
{
	int		index;
	char	quote;

	index = 0;
	quote = '\0';
	while (line[index])
	{
		if ((line[index] == '\'' || line[index] == '\"') && !quote)
			quote = line[index];
		else if (line[index] == quote)
			quote = '\0';
		index = index + 1;
	}
	if (quote)
		return (1);
	return (0);
}
