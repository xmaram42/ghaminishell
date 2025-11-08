/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:29:29 by ghsaad            #+#    #+#             */
/*   Updated: 2025/10/27 18:29:30 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ---------- small helpers ---------- */

int strings_equal(const char *a, const char *b)
{
	if (!a || !b)
		return (0);
	return (ft_strcmp(a, b) == 0);
}
// clear_builtin
int	clear_builtin(void)
{
	write(1, "\033[2J\033[H", 7);
	return (0);
}

int builtin_echo(t_cmd *command)
{
	return (ft_echo(command->argv));
}