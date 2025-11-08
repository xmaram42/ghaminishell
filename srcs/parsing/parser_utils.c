/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:29:31 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/06 16:13:51 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pars_word(t_cmd *cmd, t_token *tok, t_data *data)
{
	int		argc;
	int		i;
	char	**new_argv;
	char	*expanded;
	char	**env_arr;

	argc = 0;
	if (cmd->argv)
		while (cmd->argv[argc])
			argc++;
	new_argv = malloc(sizeof(char *) * (argc + 2));
	if (!new_argv)
		return ;
	i = -1;
	while (++i < argc)
		new_argv[i] = cmd->argv[i];
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
void free_cmds(t_cmd **cmds)
{
    t_cmd *tmp;
    int    i;

    if (!cmds || !*cmds)
        return; // nothing to free

    while (*cmds)
    {
        tmp = (*cmds)->next;

        if ((*cmds)->argv)
        {
            i = 0;
            while ((*cmds)->argv[i])
            {
                free((*cmds)->argv[i]);
                i++;
            }
            free((*cmds)->argv);
        }
        if ((*cmds)->infile > 2)
            close((*cmds)->infile);
        if ((*cmds)->outfile > 2)
            close((*cmds)->outfile);

        free(*cmds);
        *cmds = tmp;
    }

    *cmds = NULL; // prevent double free or dangling pointer
}

/*
** Allocate and initialize a new command node.
*/
t_cmd *new_cmd(void)
{
    t_cmd *cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->argv = NULL;
    cmd->infile = -1;   // CHANGED: Use -1 as "not set" instead of STDIN_FILENO
    cmd->outfile = -1;  // CHANGED: Use -1 as "not set" instead of STDOUT_FILENO
    cmd->next = NULL;
    cmd->skip_cmd = false;
    return (cmd);
}