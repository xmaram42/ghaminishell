/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:51:21 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/19 17:59:45 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_plain_break(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
        if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == ';')
                return (true);
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}


static bool	is_word_stop(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
        if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == ';')
                return (true);
	return (false);
}




static char	*wrap_with_marker(char *content, char marker)
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

	if (part == NULL)
		return (-1);
	if (*part == '\0')
	{
		free(part);
		return (0);
	}
	tmp = ft_strjoin(*word, part);
	free(*word);
	free(part);
	if (tmp == NULL)
		return (-1);
	*word = tmp;
	return (0);
}

static char	*remove_plain_escape(char *segment)
{
	char		*clean;
	size_t		i;
	size_t		j;

	clean = malloc(ft_strlen(segment) + 1);
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (segment[i])
	{
		if (segment[i] == '\\' && segment[i + 1])
			i++;
		clean[j++] = segment[i];
		i++;
	}
	clean[j] = '\0';
	free(segment);
	return (clean);
}


static int	read_plain_segment(char *line, int *index, char **part)
{
	int	start;

	start = *index;
	while (line[*index] && !is_plain_break(line[*index]))
		*index = *index + 1;
	*part = ft_substr(line, start, *index - start);
	if (!*part)
		return (-1);
	*part = remove_plain_escape(*part);
	if (!*part)
		return (-1);
	return (0);
}

int	handle_word(t_token **head, char *line, int index)
{
	char	*word;
	char	*part;
	t_token	*tok;

	word = ft_strdup("");
	if (word == NULL)
		return (-1);
	while (line[index] && !is_word_stop(line[index]))
	{
		if (line[index] == '\'' || line[index] == '\"')
		{
			index = get_quoted_str(line, index, &part);
			if (index == -1)
				return (free(word), -1);
		}
		else if (read_plain_segment(line, &index, &part) == -1)
			return (free(word), -1);
		if (append_part(&word, part) == -1)
			return (free(word), -1);
	}
	tok = new_token(word, TOK_CMD);
	if (tok == NULL)
		return (free(word), -1);
	add_token_back(head, tok);
	return (index);
}


int	get_quoted_str(char *line, int index, char **out)
{
	char	quote;
	int	start;

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
	int	index;
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