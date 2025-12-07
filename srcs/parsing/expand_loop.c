/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 16:28:39 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/05 16:29:37 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_handle_step(char *word, t_expand *exp)
{
	int	prev_index;
	int	status;

	prev_index = exp->index;
	if (!handle_marker(word, &exp->index, &exp->in_single))
	{
		status = process_dollar(word, exp);
		if (status == -1)
			return (-1);
		if (status == 0 && handle_regular_char(word, exp) == -1)
			return (-1);
	}
	if (exp->index == prev_index)
		exp->index++;
	return (0);
}

char	*expand_loop(char *word, char **envp, int last_exit)
{
	t_expand	exp;

	exp.result = ft_strdup("");
	if (!exp.result)
		return (NULL);
	exp.index = 0;
	exp.in_single = false;
	exp.last_exit = last_exit;
	exp.envp = envp;
	while (word[exp.index])
	{
		if (expand_handle_step(word, &exp) == -1)
			return (free(exp.result), NULL);
	}
	return (exp.result);
}
