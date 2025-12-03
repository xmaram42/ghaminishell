/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fd_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 14:34:38 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/03 14:07:05 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_cmd_fds(t_cmd *cmd, int type, int fd)
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

char	*prepare_redir_name(t_token *tok, t_data *data, int type,
	bool *expand_delim)
{
	char	*name;

	*expand_delim = true;
	name = expand_redir_word(tok->next, data, type, expand_delim);
	return (name);
}

int	open_redir_fd(int type, char *name, bool expand_delim,
	t_data *data)
{
	int	fd;

	fd = -1;
	if (type == TOK_HEREDOC)
		fd = handle_heredoc(name, expand_delim, data);
	else
		fd = open_redir_file(type, name);
	return (fd);
}

void	parse_redir(t_cmd *cmd, t_token *tok, t_data *data)
{
	int		fd;
	int		type;
	bool	expand_delim;
	char	*name;

	if (!cmd || !tok || !data || cmd->skip_cmd)
		return ;
	if (!validate_redir_target(cmd, tok, data))
		return ;
	type = tok->type;
	name = prepare_redir_name(tok, data, type, &expand_delim);
	if (!name)
		return ((void)(cmd->skip_cmd = true));
	fd = open_redir_fd(type, name, expand_delim, data);
	free(name);
	if (fd < 0)
	{
		cmd->skip_cmd = true;
		if (data->exit_code != 130)
			data->exit_code = 1;
		return ;
	}
	update_cmd_fds(cmd, type, fd);
}
