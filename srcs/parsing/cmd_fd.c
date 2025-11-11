/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 10:52:46 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/10 18:30:00 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_redir_error(t_cmd *cmd, t_data *data, char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token && *token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("'", 2);
	cmd->skip_cmd = true;
	data->exit_code = 258;
}

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

static int	ensure_target_token(t_cmd *cmd, t_token *tok, t_data *data)
{
	if (!tok->next)
	{
		print_redir_error(cmd, data, NULL);
		return (0);
	}
	if (tok->next->type != TOK_CMD)
	{
		print_redir_error(cmd, data, tok->next->str);
		return (0);
	}
	return (1);
}

static char	**dup_envp_or_empty(t_data *data)
{
	if (!data->env)
		return (ft_calloc(1, sizeof(char *)));
	return (lst_to_arr(data->env));
}

static char	*expand_redir_word(t_token *tok, t_data *data, int type)
{
	char	**env_arr;
	char	*expanded;

	if (type == TOK_HEREDOC)
		return (strip_markers(tok->str));
	env_arr = dup_envp_or_empty(data);
	if (!env_arr)
	{
		print_error(ERR_MALLOC);
		data->exit_code = 1;
		return (NULL);
	}
	expanded = expand_value(tok->str, env_arr, data->exit_code);
	free_array(env_arr);
	if (!expanded)
	{
		print_error(ERR_MALLOC);
		data->exit_code = 1;
	}
	return (expanded);
}

void	parse_redir(t_cmd *cmd, t_token *tok, t_data *data)
{
	int	fd;
	char	*name;
	int	type;

	if (cmd->skip_cmd)
		return ;
	if (!ensure_target_token(cmd, tok, data))
		return ;
	type = tok->type;
	name = expand_redir_word(tok->next, data, type);
	if (!name)
	{
		cmd->skip_cmd = true;
		return ;
	}
	fd = open_target(type, name, data);
	free(name);
	if (fd < 0)
	{
		cmd->skip_cmd = true;
		data->exit_code = 1;
		return ;
	}
	apply_fd(cmd, type, fd);
}
