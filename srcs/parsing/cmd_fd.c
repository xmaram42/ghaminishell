/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:46 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/06 17:44:03 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redir_file(int type, char *name)
{
	int	fd;

	fd = -1;
	if (type == TOK_REDIR_IN)
		fd = open(name, O_RDONLY);
	else if (type == TOK_REDIR_OUT)
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == TOK_APPEND)
		fd = open(name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		perror(name);
	return (fd);
}

static int	open_target(int type, char *name, t_data *data)
{
	if (type == TOK_HEREDOC)
		return (handle_heredoc(name, data));
	return (open_redir_file(type, name));
}

static void	apply_fd(t_cmd *cmd, int type, int fd)
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
	char	*name;
	int		type;

	if (cmd->skip_cmd)
		return ;
	if (!tok->next || tok->next->type != TOK_CMD)
		return ;
	name = tok->next->str;
	type = tok->type;
	fd = open_target(type, name, data);
	if (fd < 0)
	{
		cmd->skip_cmd = true;
		data->exit_code = 1;
		return ;
	}
	apply_fd(cmd, type, fd);
}