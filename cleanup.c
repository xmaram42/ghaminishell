/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:52:40 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 13:16:38 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	cleanup_philosophers(t_data *data)
{
	unsigned int	i;

	if (data->philos)
	{
		i = 0;
		while (i < data->n)
		{
			pthread_mutex_destroy(&data->philos[i].meal_lock);
			i++;
		}
		free(data->philos);
		data->philos = NULL;
	}
}

static void	cleanup_forks(t_data *data)
{
	unsigned int	i;

	if (data->fork_mutex)
	{
		i = 0;
		while (i < data->n)
		{
			pthread_mutex_destroy(&data->fork_mutex[i]);
			i++;
		}
		free(data->fork_mutex);
		data->fork_mutex = NULL;
	}
}

void	cleanup(t_data *data)
{
	cleanup_philosophers(data);
	cleanup_forks(data);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->eat_mutex);
	pthread_mutex_destroy(&data->die_mutex);
}
