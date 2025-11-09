/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:08:58 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/09 18:36:58 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ===== helper: get value of variable ===== */
static char	*get_var_value(char *n, char **envp, int last_exit)
{
	char	*v;

	if (n[0] == '?' && n[1] == '\0')
		return (ft_itoa(last_exit));
        if (ft_isdigit(n[0]))
                return (ft_strdup(""));
        if (!ft_isalpha(n[0]) && n[0] != '_')
                return (ft_strjoin("$", n)); // literal, not expanded
	v = find_env_value(n, envp);
	if (v)
		return (ft_strdup(v));
	return (ft_strdup(""));
}

static int	parse_braced_var(char *w, int i, char **name)
{
	int	j;

	j = i + 2;
	while (w[j] && w[j] != '}')
		j++;
	if (!w[j])
	{
		*name = ft_strdup("");
		return (-2);
	}
	*name = ft_substr(w, i + 2, j - i - 2);
	return (j + 1);
}

/* ===== main: handle $ and ${VAR} ===== */
static int	handle_dollar(char *w, int i,
	char **res, char **envp, int last_exit)
{
	char	*n;
	char	*v;
	int		next;

	if (w[i + 1] == '{')
	{
		next = parse_braced_var(w, i, &n);
		if (next == -2) // missing closing brace
			return (append_part(res, ft_substr(w, i, ft_strlen(w) - i)), ft_strlen(w));
		i = next;
	}
	else
	{
		n = extract_var_name(w, i);
		i += ft_strlen(n) + 1;
	}
	v = get_var_value(n, envp, last_exit);
	if (!v || append_part(res, v) == -1)
		return (free(n), -1);
	free(n);
	return (i);
}


static char	*expand_loop(char *word, char **envp, int last_exit)
{
	char	*res;
	int		i;
	int		next;
	bool	in_single;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	in_single = false;
	while (word[i])
	{
		if (word[i] == SQ_MARKER)
		{
			in_single = !in_single;
			i++;
			continue ;
		}
		if (word[i] == DQ_MARKER)
		{
			i++;
			continue ;
		}
		if (word[i] == '$' && !in_single)
		{
			next = handle_dollar(word, i, &res, envp, last_exit);
			if (next == -1)
				return (free(res), NULL);
			i = next;
			continue ;
		}
		if (append_part(&res, ft_substr(word, i, 1)) == -1)
		{
			free(res);
			return (NULL);
		}
		i++;
	}
	return (res);
}

char	*expand_value(char *word, char **envp, int last_exit)
{
	char	*tmp;
	char	*res;

	tmp = expand_tilde(word, envp);
	if (!tmp)
		return (NULL);
	res = expand_loop(tmp, envp, last_exit);
	free(tmp);
	return (res);
}
