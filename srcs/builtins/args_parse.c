/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 12:19:01 by ghsaad            #+#    #+#             */
/*   Updated: 2025/10/30 17:02:46 by ghsaad           ###   ########.fr       */
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