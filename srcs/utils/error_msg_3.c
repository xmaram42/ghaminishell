/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:47:09 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/01 00:26:33 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_heredoc(char *delimiter)
{
	(void)delimiter;
}

void	err_allocation(void)
{
	err_prefix();
	ft_putendl_fd("malloc: cannot allocate memory", STDERR_FILENO);
}

void	err_env_init(void)
{
	err_prefix();
	ft_putendl_fd("environment initialization failed", STDERR_FILENO);
}

void	dispatch_system_errors(t_error_type type, char *subject,
							char *detail, int errnum)
{
	if (type == ERR_FORK)
		err_with_errno("fork", NULL, errnum);
	else if (type == ERR_PIPE)
		err_with_errno("pipe", NULL, errnum);
	else if (type == ERR_OPEN_FILE)
		err_with_errno(NULL, detail, errnum);
	else if (type == ERR_CHDIR)
		err_with_errno(subject, detail, errnum);
	else if (type == ERR_EXECVE)
		err_with_errno(NULL, detail, errnum);
	else if (type == ERR_WAIT)
		err_with_errno("waitpid", NULL, errnum);
	else if (type == ERR_NO_SUCH_FILE)
		err_with_errno(NULL, subject, ENOENT);
	else if (type == ERR_PERMISSION)
		err_with_errno(NULL, subject, EACCES);
	else if (type == ERR_IS_DIRECTORY)
		err_with_errno(NULL, subject, EISDIR);
	else if (type == ERR_GENERAL)
		err_with_errno(subject, detail, errnum);
}

int	handle_primary_errors(t_error_type type, char *subject,
	char *detail)
{
	if (type == ERR_CMD_NOT_FOUND)
		err_cmd_not_found(subject);
	else if (type == ERR_SYNTAX)
		err_unexpected_token(detail);
	else if (type == ERR_UNCLOSED_QUOTE)
		err_unclosed_quote();
	else if (type == ERR_NUMERIC_ARG)
		err_numeric(subject, detail);
	else if (type == ERR_TOO_MANY_ARGS)
		err_too_many(subject);
	else if (type == ERR_INVALID_IDENTIFIER)
		err_invalid_identifier(subject, detail);
	else if (type == ERR_AMBIGUOUS_REDIRECT)
		err_simple_subject(NULL, "ambiguous redirect");
	else
		return (0);
	return (1);
}
