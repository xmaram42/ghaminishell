/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:12:44 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 11:13:30 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	create_philo_threads(t_data *data)
{
	unsigned int	i;

	data->start_time = (long long)get_time_ms();
	i = 0;
	while (i < data->n)
	{
		data->philos[i].last_meal = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				&philo_routine, &data->philos[i]) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_join(data->philos[i].thread, NULL);
			}
			return (false);
		}
		i++;
	}
	return (true);
}

static void	join_philo_threads(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->n)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

bool	start_philo(t_data *data)
{
	if (!create_philo_threads(data))
		return (false);
	monitor_philos(data);
	join_philo_threads(data);
	return (true);
}
