/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:45:59 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 00:25:19 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_numeric(char *subject, char *detail)
{
	err_prefix_subject(subject);
	if (detail && *detail)
	{
		ft_putstr_fd(detail, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd("numeric argument required", STDERR_FILENO);
}

void	err_too_many(char *subject)
{
	err_prefix_subject(subject);
	ft_putendl_fd("too many arguments", STDERR_FILENO);
}

void	err_invalid_identifier(char *subject, char *identifier)
{
	err_prefix_subject(subject);
	ft_putstr_fd("`", STDERR_FILENO);
	if (identifier && *identifier)
		ft_putstr_fd(identifier, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

void	err_simple_subject(char *subject, char *message)
{
	err_prefix_subject(subject);
	ft_putendl_fd(message, STDERR_FILENO);
}

void	err_with_errno(char *subject, char *detail, int errnum)
{
	if (!errnum)
		errnum = errno;
	err_prefix_subject(subject);
	if (detail && *detail)
	{
		ft_putstr_fd(detail, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
