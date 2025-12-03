/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:54:41 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 00:23:32 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_n_flags(char **args)
{
	int	i;
	int	j;
	int	newline_flag;

	i = 1;
	newline_flag = 1;
	while (args[i] && args[i][0] == '-' && args[i][1])
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		newline_flag = 0;
		i++;
	}
	return (newline_flag << 16 | i);
}

int	ft_echo(char **args)
{
	int	i;
	int	newline_flag;
	int	result;

	if (!args[1])
	{
		printf("\n");
		return (0);
	}
	result = parse_n_flags(args);
	newline_flag = result >> 16;
	i = result & 0xFFFF;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline_flag)
		printf("\n");
	return (0);
}
