/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:23:43 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/09 18:37:23 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int      is_marker(char c)
{
        return (c == SQ_MARKER || c == DQ_MARKER);
}

char    *strip_markers(const char *word)
{
        char    *clean;
        size_t  i;
        size_t  j;

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
                        clean[j++] = word[i];
                i++;
        }
        clean[j] = '\0';
        return (clean);
}

char    *find_env_value(char *key, char **envp)
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
        if (ft_isdigit(str[i]))
        {
                while (str[i] && ft_isdigit(str[i]))
                        i++;
                len = i - (start + 1);
                return (ft_substr(str, start + 1, len));
        }
        if (!ft_isalpha(str[i]) && str[i] != '_')
                return (ft_strdup("")); // invalid start like $-, $@
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
