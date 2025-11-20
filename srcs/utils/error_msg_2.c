/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:48:00 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/20 13:57:03 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_state_errors(t_error_type type, char *subject,
						char *detail)
{
	if (type == ERR_HOME_NOT_SET)
		err_simple_subject(subject, "HOME not set");
	else if (type == ERR_OLDPWD_NOT_SET)
		err_simple_subject(subject, "OLDPWD not set");
	else if (type == ERR_ENV_NOT_SET)
		err_simple_subject(subject, "no environment");
	else if (type == ERR_HEREDOC_EOF)
		err_heredoc(detail);
	else if (type == ERR_ENV_INIT)
		err_env_init();
	else if (type == ERR_ALLOCATION)
		err_allocation();
	else
		return (0);
	return (1);
}

void	error_type_msg(t_error_type type, char *subject, char *detail,
						int errnum)
{
	if (handle_primary_errors(type, subject, detail))
		return ;
	if (handle_state_errors(type, subject, detail))
		return ;
	dispatch_system_errors(type, subject, detail, errnum);
}
