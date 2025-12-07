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
