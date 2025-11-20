/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:44:07 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 11:07:23 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_prefix(void)
{
	ft_putstr_fd("lolipop🍭: ", STDERR_FILENO);
}

void	err_prefix_subject(char *subject)
{
	err_prefix();
	if (subject && *subject)
	{
		ft_putstr_fd(subject, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
}

void	err_cmd_not_found(char *cmd)
{
	err_prefix_subject(cmd);
	ft_putendl_fd("command not found", STDERR_FILENO);
}

void	err_unexpected_token(char *token)
{
	err_prefix();
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
	if (token && *token)
		ft_putstr_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

void	err_unclosed_quote(void)
{
	err_prefix();
	ft_putendl_fd("syntax error: unexpected end of file", STDERR_FILENO);
}
