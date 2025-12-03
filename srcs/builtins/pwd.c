/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:47:34 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/01 00:21:56 by maram            ###   ########.fr       */
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
		error_type_msg(ERR_GENERAL, "pwd", NULL, errno);
		return (1);
	}
}
