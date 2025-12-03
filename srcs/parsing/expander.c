/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:08:58 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/03 13:53:17 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_dollar(char *word, t_expand *exp)
{
	int	next;

	if (word[exp->index] != '$' || exp->in_single)
		return (0);
	next = handle_dollar(word, exp->index, exp);
	if (next == -1)
		return (-1);
	exp->index = next;
	return (1);
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

static int	handle_regular_char(char *word, t_expand *exp)
{
	if (append_part(&exp->result, ft_substr(word, exp->index, 1)) == -1)
		return (-1);
	exp->index++;
	return (0);
}

static char	*expand_loop(char *word, char **envp, int last_exit)
{
	t_expand	exp;
	int			status;

	exp.result = ft_strdup("");
	if (!exp.result)
		return (NULL);
	exp.index = 0;
	exp.in_single = false;
	exp.last_exit = last_exit;
	exp.envp = envp;
	while (word[exp.index])
	{
		if (!handle_marker(word, &exp.index, &exp.in_single))
		{
			status = process_dollar(word, &exp);
			if (status == -1)
				return (free(exp.result), NULL);
			if (status == 0 && handle_regular_char(word, &exp) == -1)
				return (free(exp.result), NULL);
		}
	}
	return (exp.result);
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
