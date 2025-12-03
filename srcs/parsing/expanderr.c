/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanderr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deep <deep@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:08:58 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 00:54:11 by deep             ###   ########.fr       */
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
	int		length;

	length = ft_strlen(word) - index;
	tail = ft_substr(word, index, length);
	free(name);
	if (!tail)
		return (-1);
	if (append_part(result, tail) == -1)
		return (-1);
	return (ft_strlen(word));
}

int	handle_dollar(char *word, int index, t_expand *exp)
{
	char	*name;
	char	*value;
	int		next;

	next = read_variable(word, index, &name);
	if (next == -1)
		return (-1);
	if (next == -2)
		return (append_missing_brace(word, index, &exp->result, name));
	value = get_var_value(name, exp->envp, exp->last_exit);
	free(name);
	if (!value)
		return (-1);
	if (append_part(&exp->result, value) == -1)
		return (-1);
	return (next);
}
