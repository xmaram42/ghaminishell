/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:34:38 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/19 14:35:44 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	open_redir_file(int type, char *name)
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

static int	validate_redir_target(t_cmd *cmd, t_token *tok, t_data *data)
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

static char	*expand_redir_word(t_token *tok, t_data *data, int type)
{
	char	**env_arr;
	char	*expanded;

	if (!tok)
		return (NULL);
	if (type == TOK_HEREDOC)
		return (strip_markers(tok->str));
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

static void	update_cmd_fds(t_cmd *cmd, int type, int fd)
{
	if (type == TOK_REDIR_IN || type == TOK_HEREDOC)
	{
		if (cmd->infile >= 0)
			close(cmd->infile);
		cmd->infile = fd;
	}
	else
	{
		if (cmd->outfile >= 0)
			close(cmd->outfile);
		cmd->outfile = fd;
	}
}

void	parse_redir(t_cmd *cmd, t_token *tok, t_data *data)
{
	int		fd;
	int		type;
	char	*name;

	if (!cmd || !tok || !data || cmd->skip_cmd)
		return ;
	if (!validate_redir_target(cmd, tok, data))
		return ;
	type = tok->type;
	name = expand_redir_word(tok->next, data, type);
	if (!name)
	{
		cmd->skip_cmd = true;
		return ;
	}
	fd = -1;
	if (type == TOK_HEREDOC)
		fd = handle_heredoc(name, data);
	else
		fd = open_redir_file(type, name);
	free(name);
	if (fd < 0)
	{
		cmd->skip_cmd = true;
		data->exit_code = 1;
		return ;
	}
	update_cmd_fds(cmd, type, fd);
}
