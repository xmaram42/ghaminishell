/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:51:21 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/09 18:19:50 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *wrap_with_marker(char *content, char marker)
{
	char	*wrapped;
	size_t	len;

	if (!content)
		return (NULL);
	len = ft_strlen(content);
	wrapped = malloc(len + 3);
	if (!wrapped)
	{
		free(content);
		return (NULL);
	}
	wrapped[0] = marker;
	if (len)
		ft_memcpy(wrapped + 1, content, len);
	wrapped[len + 1] = marker;
	wrapped[len + 2] = '\0';
	free(content);
	return (wrapped);
}

int	append_part(char **word, char *part)
{
	char	*tmp;

	if (!part)
		return (-1);
	if (*part == '\0')  // 👈 skip if nothing to add
	{
		free(part);
		return (0);
	}
	tmp = ft_strjoin(*word, part);
	free(*word);
	free(part);
	if (!tmp)
		return (-1);
	*word = tmp;
	return (0);
}
int	handle_word(t_token **head, char *line, int i)
{
	char	*word;
	char	*part;
	int		start;

	word = ft_strdup("");
	if (!word)
		return (-1);
	while (line[i] && !ft_strchr(" \t|<>", line[i]))
	{
		if (line[i] == '\'' || line[i] == '"')
			i = get_quoted_str(line, i, &part);
		else
		{
			start = i;
			while (line[i] && !ft_strchr(" \t|<>'\"", line[i]))
				i++;
			part = ft_substr(line, start, i - start);
		}
		if (i == -1 || append_part(&word, part) == -1)
			return (free(word), -1);
	}
	add_token_back(head, new_token(word, TOK_CMD));
	return (i);
}

int get_quoted_str(char *line, int i, char **out)
{
	char 	quote;
	int 	start;

	quote = line[i];
	i++;
	start = i;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == '\0')
		return (-1);
	*out = ft_substr(line, start, i - start);
	if (!*out)
		return (-1);
	if (quote == '\'')
		*out = wrap_with_marker(*out, SQ_MARKER);
	else
		*out = wrap_with_marker(*out, DQ_MARKER);
	if (!*out)
		return (-1);
	return (i + 1);
}
int	has_unclosed_quote(const char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = '\0';
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = '\0';
		i++;
	}
	return (quote != '\0');
}
