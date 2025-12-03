/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deep <deep@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:51:21 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/30 23:54:35 by deep             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*wrap_with_marker(char *content, char marker)
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
