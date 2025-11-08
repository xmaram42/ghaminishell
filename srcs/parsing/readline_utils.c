/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:33:37 by aalbugar          #+#    #+#             */
/*   Updated: 2025/10/27 14:09:11 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char *read_full_line(void)
// {
// 	char *line;
// 	char *next;
// 	char *tmp;

// 	line = readline("lolipop 🍭$ ");
// 	if (!line)
// 	{
// 		ft_putendl_fd("exit", 1);
// 		return (NULL);
// 	}
// 	while (has_unclosed_quote(line))
// 	{
// 		next = readline("> ");
// 		if (!next)
// 		{
// 			ft_putstr_fd("lolipop 🍭$ : unexpected EOF\n", 2);
// 			free(line);
// 			return (NULL);
// 		}
// 		tmp = ft_strjoin(line, "\n");
// 		free(line);
// 		line = ft_strjoin(tmp, next);
// 		free(tmp);
// 		free(next);
// 	}
// 	return (line);
// }
// This function reads a full line from the user, handling multi-line input if there are unclosed quotes.


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
		ft_putstr_fd("lolipop 🍭$ : unexpected EOF\n", 2);
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
