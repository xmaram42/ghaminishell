/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:54:41 by ghsaad            #+#    #+#             */
/*   Updated: 2025/10/13 12:05:29 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
this is a simple implementation of the echo command
its main point is to handle the -n flag and print the arguments separated by spaces 
*/
int	ft_echo(char **args)
{
	int	i;			
	int	newline_flag;
	
	i = 1;			// Start from 1 to skip the command name "echo"
	newline_flag = 1; // By default, we want to print a newline at the end
	if (!args[1]) // If no arguments are provided, just print a newline
	{
		printf("\n");
		return (0);
	}
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n'
			&& args[i][2] == '\0')
	{
		newline_flag = 0; // If we find -n, we set the flag to not print a newline
		i++; // so echo -n hi is hibash-3.2$ and not hi then bash on a newline
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1]) // If there's another argument, print a space
			printf(" ");
		i++;
	}
	if (newline_flag) // If the -n flag was not found, print a newline
		printf("\n");
	return (0);
}