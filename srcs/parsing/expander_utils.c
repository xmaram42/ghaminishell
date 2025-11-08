/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:23:43 by aalbugar          #+#    #+#             */
/*   Updated: 2025/10/30 16:46:03 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_env_value(char *key, char **envp)
{
	int i;
	int len;

	if(!key || !envp)
		return(NULL);
	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*extract_var_name(char *str, int start)
{
	int		i;
	int		len;

	i = start + 1;
	if (str[i] == '?')
		return (ft_strdup("?"));
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (ft_strdup("")); // invalid start like $1, $-
	i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	len = i - (start + 1);
	return (ft_substr(str, start + 1, len));
}

char	*expand_tilde(char *word, char **envp)
{
	char	*home;
	char	*expanded;
	int		i;

	if (!word)
		return (NULL);
	i = 0;
	while (word[i])
	{
		if (word[i] == '~'
			&& (i == 0 || word[i - 1] == '=')
			&& (!word[i + 1] || word[i + 1] == '/'))
		{
			home = find_env_value("HOME", envp);
			if (!home)
				return (ft_strdup(word));
			expanded = ft_strjoin(home, word + i + 1);
			if (i == 0)
				return (expanded);
			return (ft_strjoin_free(ft_substr(word, 0, i + 1), expanded));
		}
		i++;
	}
	return (ft_strdup(word));
}
/*
this function joins two strings and frees the first one.
*/
char	*ft_free_first_str(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}
