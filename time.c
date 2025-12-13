/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:58:39 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/10 17:08:27 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((unsigned long long)tv.tv_sec * 1000ULL
		+ (unsigned long long)tv.tv_usec / 1000ULL);
}

int	usleep_ms(unsigned long long time, t_data *data)
{
	unsigned long long	start;

	if (time == 0)
		return (0);
	start = get_time_ms();
	while (!simulation_stopped(data))
	{
		if (get_time_ms() - start >= time)
			break ;
		usleep(100);
	}
	return (0);
}
