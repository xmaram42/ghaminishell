/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:19:01 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/20 12:21:33 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_arg(char *start, char *end)
{
	char	*arg;
	int		len;

	if (!start || !end || end <= start)
		return (NULL);
	len = end - start;
	arg = malloc(len + 1);
	if (!arg)
		return (NULL);
	ft_strlcpy(arg, start, len + 1);
	return (arg);
}

void	skip_spaces(char **str)
{
	while (**str && **str == ' ')
		(*str)++;
}
