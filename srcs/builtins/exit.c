/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:04:05 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/03 14:08:17 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_sign_and_skip_spaces(char *str, int *i)
{
	int	pn;

	while ((9 <= str[*i] && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
	pn = 1;
	if (str[*i] == '+' || str[*i] == '-')
		if (str[(*i)++] == '-')
			pn = -1;
	return (pn);
}

static int	almost_atoi(char *str, int *err)
{
	unsigned long long	ret;
	int					i;
	int					j;
	int					pn;

	i = 0;
	pn = parse_sign_and_skip_spaces(str, &i);
	j = i;
	ret = 0;
	while ('0' <= str[i] && str[i] <= '9')
		ret = ret * 10 + (str[i++] - 48);
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] || i - j > 20 || ((pn == -1 && (ret - 1) > LONG_MAX)
			|| (pn == 1 && (ret > LONG_MAX))))
	{
		*err = 1;
		return (255);
	}
	return ((int)((ret * pn) % 256));
}

static int	handle_exit_args(char **args, int *err)
{
	int	ret;

	ret = almost_atoi(args[1], err);
	if (*err)
	{
		error_type_msg(ERR_NUMERIC_ARG, "exit", args[1], 0);
		return (255);
	}
	return (ret);
}

void	ft_exit(t_data *data, char **args)
{
	int	ret;
	int	err;

	ret = data->exit_code;
	err = 0;
	ft_putstr_fd("exit\n", 1);
	if (args[1])
		ret = handle_exit_args(args, &err);
	if (args[1] && args[2])
	{
		error_type_msg(ERR_TOO_MANY_ARGS, "exit", NULL, 0);
		data->exit_code = 1;
		return ;
	}
	data->exit_code = ret;
	data->exit_flag = true;
}
