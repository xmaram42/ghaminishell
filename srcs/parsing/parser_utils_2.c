/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deep <deep@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:20:32 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 00:46:45 by deep             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pars_word(t_cmd *cmd, t_token *tok, t_data *data)
{
	int		argc;
	char	**new_argv;
	char	*expanded;
	char	**env_arr;

	argc = get_argc(cmd->argv);
	new_argv = create_new_argv(cmd->argv, argc);
	if (!new_argv)
		return ;
	env_arr = lst_to_arr(data->env);
	expanded = expand_value(tok->str, env_arr, data->exit_code);
	free_array(env_arr);
	if (expanded)
		new_argv[argc] = expanded;
	else
		new_argv[argc] = ft_strdup(tok->str);
	new_argv[argc + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}
/*
** Free a linked list of commands and their argv.
*/

static void	free_cmd_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	close_cmd_fds(t_cmd *cmd)
{
	if (cmd->infile > 2)
		close(cmd->infile);
	if (cmd->outfile > 2)
		close(cmd->outfile);
}

void	free_cmds(t_cmd **cmds)
{
	t_cmd	*tmp;

	if (!cmds || !*cmds)
		return ;
	while (*cmds)
	{
		tmp = (*cmds)->next;
		free_cmd_argv((*cmds)->argv);
		close_cmd_fds(*cmds);
		free(*cmds);
		*cmds = tmp;
	}
	*cmds = NULL;
}

/*
** Allocate and initialize a new command node.
*/
t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->next = NULL;
	cmd->skip_cmd = false;
	return (cmd);
}
