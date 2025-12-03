/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:34:38 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/03 14:07:00 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_redir_word_cont(t_token *tok, t_data *data)
{
	char	**env_arr;
	char	*expanded;

	env_arr = NULL;
	if (data->env)
		env_arr = lst_to_arr(data->env);
	else
		env_arr = ft_calloc(1, sizeof(char *));
	if (!env_arr)
	{
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		data->exit_code = 1;
		return (NULL);
	}
	expanded = expand_value(tok->str, env_arr, data->exit_code);
	free_array(env_arr);
	if (expanded)
		return (expanded);
	error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
	data->exit_code = 1;
	return (NULL);
}

char	*expand_redir_word(t_token *tok, t_data *data, int type,
		bool *expand_delim)
{
	if (!tok)
		return (NULL);
	if (type == TOK_HEREDOC)
	{
		if (expand_delim)
			*expand_delim = delimiter_should_expand(tok->str);
		return (strip_markers(tok->str));
	}
	return (expand_redir_word_cont(tok, data));
}

int	open_redir_file(int type, char *name)
{
	int	flags;

	if (!name)
		return (-1);
	if (type == TOK_REDIR_IN)
		return (open(name, O_RDONLY));
	flags = O_WRONLY | O_CREAT;
	if (type == TOK_REDIR_OUT)
		flags |= O_TRUNC;
	else if (type == TOK_APPEND)
		flags |= O_APPEND;
	else
		return (-1);
	return (open(name, flags, 0644));
}

int	validate_redir_target(t_cmd *cmd, t_token *tok, t_data *data)
{
	t_token	*target;
	char	*err;

	target = tok->next;
	if (target && target->type == TOK_CMD)
		return (1);
	err = NULL;
	if (target && target->str && *target->str)
		err = target->str;
	error_type_msg(ERR_SYNTAX, NULL, err, 0);
	cmd->skip_cmd = true;
	data->exit_code = 258;
	return (0);
}

bool	delimiter_should_expand(char *word)
{
	int		index;

	index = 0;
	while (word && word[index])
	{
		if (word[index] == SQ_MARKER || word[index] == DQ_MARKER)
			return (false);
		index++;
	}
	return (true);
}
