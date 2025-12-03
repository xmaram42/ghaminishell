/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deep <deep@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:51:21 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/30 23:55:33 by deep             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	process_word_segment(char *line, int *index, char **word)
{
	char	*part;
	int		result;

	if (line[*index] == '\'' || line[*index] == '\"')
	{
		result = get_quoted_str(line, *index, &part);
		if (result == -1)
			return (-1);
		*index = result;
	}
	else if (read_plain_segment(line, index, &part) == -1)
		return (-1);
	if (append_part(word, part) == -1)
		return (-1);
	return (0);
}

int	handle_word(t_token **head, char *line, int index)
{
	char	*word;
	t_token	*tok;

	word = ft_strdup("");
	if (word == NULL)
		return (-1);
	while (line[index] && !is_word_stop(line[index]))
	{
		if (process_word_segment(line, &index, &word) == -1)
			return (free(word), -1);
	}
	tok = new_token(word, TOK_CMD);
	if (tok == NULL)
		return (free(word), -1);
	add_token_back(head, tok);
	return (index);
}
