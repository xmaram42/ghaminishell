/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:33:29 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/10 17:08:34 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sg;
	unsigned long long	rus;

	i = 0;
	sg = 1;
	rus = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sg *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
		rus = rus * 10 + (str[i++] - '0');
	if (sg == -1 && rus >= 9223372036854775808ULL)
		return (0);
	if (rus >= __LONG_LONG_MAX__ && sg != -1)
		return (-1);
	return (sg * (int)rus);
}
