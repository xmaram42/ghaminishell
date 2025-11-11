/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:23:43 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/11 21:00:00 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_marker(char c)
{
	return (c == SQ_MARKER || c == DQ_MARKER);
}

char	*strip_markers(const char *word)
{
	char	*clean;
	size_t	i;
	size_t	j;

	if (!word)
		return (NULL);
	clean = malloc(ft_strlen(word) + 1);
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (word[i])
	{
		if (!is_marker(word[i]))
		{
			clean[j] = word[i];
			j = j + 1;
		}
		i = i + 1;
	}
	clean[j] = '\0';
	return (clean);
}

char	*find_env_value(char *key, char **envp)
{
	int	index;
	int	len;

	if (!key || !envp)
		return (NULL);
	index = 0;
	len = ft_strlen(key);
	while (envp[index])
	{
		if (!ft_strncmp(envp[index], key, len)
			&& envp[index][len] == '=')
			return (envp[index] + len + 1);
		index = index + 1;
	}
	return (NULL);
}

char	*extract_var_name(char *str, int start)
{
	int	index;
	int	len;

	index = start + 1;
	if (str[index] == '?')
		return (ft_strdup("?"));
	if (ft_isdigit(str[index]))
	{
		while (str[index] && ft_isdigit(str[index]))
			index = index + 1;
		len = index - (start + 1);
		return (ft_substr(str, start + 1, len));
	}
	if (!ft_isalpha(str[index]) && str[index] != '_')
		return (ft_strdup(""));
	index = index + 1;
	while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
		index = index + 1;
	len = index - (start + 1);
	return (ft_substr(str, start + 1, len));
}

char	*expand_tilde(char *word, char **envp)
{
	char	*home;
	char	*expanded;
	char	*prefix;
	int	index;

	if (!word)
		return (NULL);
	index = 0;
	while (word[index])
	{
		if (word[index] == '~'
			&& (index == 0 || word[index - 1] == '=')
			&& (!word[index + 1] || word[index + 1] == '/'))
		{
			home = find_env_value("HOME", envp);
			if (!home)
				return (ft_strdup(word));
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
		index = index + 1;
	}
	return (ft_strdup(word));
}

char	*ft_free_first_str(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}