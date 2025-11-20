/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:33:37 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 13:58:37 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_chunk(char **line, char *next)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(*line, "\n");
	if (!tmp)
	{
		free(*line);
		*line = NULL;
		free(next);
		return (0);
	}
	joined = ft_strjoin(tmp, next);
	free(tmp);
	free(*line);
	*line = joined;
	free(next);
	return (*line != NULL);
}

static int	read_continuation(char **line)
{
	char	*next;

	next = readline("> ");
	if (!next)
	{
		error_type_msg(ERR_UNCLOSED_QUOTE, NULL, NULL, 0);
		free(*line);
		*line = NULL;
		return (0);
	}
	return (append_chunk(line, next));
}

char	*read_full_line(void)
{
	char	*line;

	line = readline("lolipop 🍭$ ");
	if (!line)
	{
		ft_putendl_fd("exit", 1);
		return (NULL);
	}
	while (has_unclosed_quote(line))
		if (!read_continuation(&line))
			return (NULL);
	return (line);
}
