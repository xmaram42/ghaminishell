/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:04:05 by aalbugar          #+#    #+#             */
/*   Updated: 2025/11/17 14:05:15 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	is_numeric_str(char *s)
// {
// 	int	i;

// 	if (!s || !*s)
// 		return (0);
// 	if (s[0] == '+' || s[0] == '-')
// 		s++;
// 	i = 0;
// 	while (s[i])
// 	{
// 		if (!ft_isdigit(s[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

static int	almost_atoi(char *str, int *err)
{
	unsigned long long	ret;
	int					i;
	int					j;
	int					pn;

	i = 0;
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	pn = 1;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			pn = -1;
	j = i;
	ret = 0;
	while ('0' <= str[i] && str[i] <= '9')
		ret = ret * 10 + (str[i++] - 48);
	while ((9 <= str[i] && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] || i - j > 20 || ((pn == -1 && (ret - 1) > LONG_MAX) || \
		(pn == 1 && (ret > LONG_MAX))))
		*err = 1;
	return ((int)((ret * pn) % 256));
}

void	ft_exit(t_data *data, char **args)
{
	int	ret;
	int	err;

	ret = data->exit_code;
	err = 0;
	ft_putstr_fd("exit\n", 1);
	if (args[1])
	{
		ret = almost_atoi(args[1], &err);
		if (err)
		{
			ft_putstr_fd("lolipop🍭: exit: numeric argument required\n", 2);
			ret = 2;
		}
	}
	if (args[1] && args[2])
	{
		ft_putstr_fd("lolipop🍭: exit: too many arguments\n", 2);
		data->exit_code = 1;
		return ;
	}
	data->exit_code = ret;
    data->exit_flag = true;
	// NOTE: no free_cmds/free_token/free_list/rl_clear_history here
	// teardown will run once after the loop ends
}
