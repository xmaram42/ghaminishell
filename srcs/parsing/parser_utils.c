/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deep <deep@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:29:31 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 00:47:36 by deep             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_argc(char **argv)
{
	int	argc;

	argc = 0;
	if (argv)
		while (argv[argc])
			argc++;
	return (argc);
}

char	**create_new_argv(char **old_argv, int argc)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (argc + 2));
	if (!new_argv)
		return (NULL);
	i = -1;
	while (++i < argc)
		new_argv[i] = old_argv[i];
	return (new_argv);
}

void	pars_word(t_cmd *cmd, t_token *tok, t_data *data)
{
    int		argc;
    char	**new_argv;
    char	*expanded;
    char	**env_arr;

    argc = get_argc(cmd->argv);
    env_arr = lst_to_arr(data->env);
    expanded = expand_value(tok->str, env_arr, data->exit_code);
    free_array(env_arr);
    if (expanded && expanded[0] == '\0')
    {
        free(expanded);
        return ;
    }
    new_argv = create_new_argv(cmd->argv, argc);
    if (!new_argv)
        return ;
    if (expanded)
        new_argv[argc] = expanded;
    else
        new_argv[argc] = ft_strdup(tok->str);
    new_argv[argc + 1] = NULL;
    free(cmd->argv);
    cmd->argv = new_argv;
}