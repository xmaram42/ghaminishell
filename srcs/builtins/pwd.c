/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:47:34 by ghsaad            #+#    #+#             */
/*   Updated: 2025/09/23 12:50:36 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	curr_working_dir[PATH_MAX];

	if (getcwd(curr_working_dir, PATH_MAX))
	{
		printf("%s\n", curr_working_dir);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
/*
This function emulates how the pwd command works,
we create a char array to hold the current working directory path,
then we use getcwd to fill that array with the current working directory.
PATH_MAX is a constant from limits.h that degines the max length of a path
*/
