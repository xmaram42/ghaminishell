/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expande_tilde.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:57:14 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 15:02:52 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_tilde_case(char *word, int index, char *home)
{
	char	*expanded;
	char	*prefix;

	expanded = ft_strjoin(home, word + index + 1);
	if (index == 0)
		return (expanded);
	prefix = ft_substr(word, 0, index + 1);
	if (!prefix)
	{
		free(expanded);
		return (NULL);
	}
	return (ft_strjoin_free(prefix, expanded));
}

static bool	is_valid_tilde_position(char *word, int index)
{
	return ((index == 0 || word[index - 1] == '=')
		&& (!word[index + 1] || word[index + 1] == '/'));
}

char	*expand_tilde(char *word, char **envp)
{
	char	*home;
	int		index;

	if (!word)
		return (NULL);
	index = 0;
	while (word[index])
	{
		if (word[index] == '~' && is_valid_tilde_position(word, index))
		{
			home = find_env_value("HOME", envp);
			if (!home)
				return (ft_strdup(word));
			return (expand_tilde_case(word, index, home));
		}
		index = index + 1;
	}
	return (ft_strdup(word));
}
