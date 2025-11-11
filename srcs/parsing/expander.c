/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:08:58 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/11 21:40:00 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_value(char *name, char **envp, int last_exit)
{
	char	*value;

	if (name[0] == '?' && name[1] == '\0')
		return (ft_itoa(last_exit));
	if (ft_isdigit(name[0]))
		return (ft_strdup(""));
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (ft_strjoin("$", name));
	value = find_env_value(name, envp);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static int	parse_braced_var(char *word, int index, char **name)
{
	int	position;

	position = index + 2;
	while (word[position] && word[position] != '}')
		position = position + 1;
	if (!word[position])
	{
		*name = ft_strdup("");
		return (-2);
	}
	*name = ft_substr(word, index + 2, position - index - 2);
	if (!*name)
		return (-1);
	return (position + 1);
}

static int	read_variable(char *word, int index, char **name)
{
	int	next;

	if (word[index + 1] == '{')
		return (parse_braced_var(word, index, name));
	*name = extract_var_name(word, index);
	if (!*name)
		return (-1);
	next = ft_strlen(*name);
	return (index + next + 1);
}

static int	append_missing_brace(char *word, int index, char **result,
		char *name)
{
	char	*tail;
	int	length;

	length = ft_strlen(word) - index;
	tail = ft_substr(word, index, length);
	free(name);
	if (!tail)
		return (-1);
	if (append_part(result, tail) == -1)
		return (-1);
	return (ft_strlen(word));
}

static int	handle_dollar(char *word, int index, char **result,
		char **envp, int last_exit)
{
	char	*name;
	char	*value;
	int	next;

	next = read_variable(word, index, &name);
	if (next == -2)
		return (append_missing_brace(word, index, result, name));
	if (next == -1)
		return (-1);
	value = get_var_value(name, envp, last_exit);
	free(name);
	if (!value)
		return (-1);
	if (append_part(result, value) == -1)
		return (-1);
	return (next);
}

static bool	handle_marker(char *word, int *index, bool *in_single)
{
	if (word[*index] == SQ_MARKER)
	{
		*in_single = !*in_single;
		*index = *index + 1;
		return (true);
	}
	if (word[*index] == DQ_MARKER)
	{
		*index = *index + 1;
		return (true);
	}
	return (false);
}

static int	process_dollar(char *word, int *index, char **result,
		char **envp, int last_exit, bool in_single)
{
	int	next;

	if (word[*index] != '$' || in_single)
		return (0);
	next = handle_dollar(word, *index, result, envp, last_exit);
	if (next == -1)
		return (-1);
	*index = next;
	return (1);
}

static char	*expand_loop(char *word, char **envp, int last_exit)
{
	char	*result;
	int	index;
	int	status;
	bool	in_single;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	index = 0;
	in_single = false;
	while (word[index])
	{
		if (handle_marker(word, &index, &in_single))
			continue ;
		status = process_dollar(word, &index, &result,
			envp, last_exit, in_single);
		if (status == -1)
			return (free(result), NULL);
		if (status == 1)
			continue ;
		if (append_part(&result, ft_substr(word, index, 1)) == -1)
			return (free(result), NULL);
		index = index + 1;
	}
	return (result);
}

char	*expand_value(char *word, char **envp, int last_exit)
{
	char	*tmp;
	char	*result;

	tmp = expand_tilde(word, envp);
	if (!tmp)
		return (NULL);
	result = expand_loop(tmp, envp, last_exit);
	free(tmp);
	return (result);
}