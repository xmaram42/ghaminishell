/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:45:26 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 14:48:19 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_assignment_word(const char *word)
{
	int	i;

	if (!word || !ft_strchr(word, '='))
		return (false);
	if (word[0] != '_' && !ft_isalpha((unsigned char)word[0]))
		return (false);
	i = 0;
	while (word[i] && word[i] != '=')
	{
		if (!ft_isalnum((unsigned char)word[i]) && word[i] != '_')
			return (false);
		i++;
	}
	return (word[i] == '=');
}

static bool	apply_assignment(t_data *data, char *assignment)
{
	if (!export(assignment, &data->env))
	{
		data->exit_code = 1;
		error_type_msg(ERR_ALLOCATION, NULL, NULL, 0);
		return (false);
	}
	return (true);
}

static void	shift_arguments(char **argv, int skip)
{
	int	i;

	i = 0;
	while (argv[skip + i])
	{
		argv[i] = argv[skip + i];
		i++;
	}
	argv[i] = NULL;
}

static bool	process_command_assignments(t_data *data, t_cmd *cmd)
{
	int	count;

	if (!cmd->argv)
		return (true);
	count = 0;
	while (cmd->argv[count] && is_assignment_word(cmd->argv[count]))
	{
		if (!apply_assignment(data, cmd->argv[count]))
			return (false);
		free(cmd->argv[count]);
		cmd->argv[count] = NULL;
		count++;
	}
	if (count == 0)
		return (true);
	shift_arguments(cmd->argv, count);
	if (!cmd->argv[0])
	{
		cmd->skip_cmd = true;
		ms_set_exit_status(data, 0);
	}
	return (true);
}

bool	prepare_assignments(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmds;
	while (cmd)
	{
		if (!process_command_assignments(data, cmd))
			return (false);
		cmd = cmd->next;
	}
	return (true);
}
