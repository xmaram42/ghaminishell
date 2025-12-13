/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:46:23 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 11:27:54 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->eat_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->die_mutex, NULL) != 0)
		return (false);
	return (true);
}

static bool	init_fork_mutexes(t_data *data)
{
	unsigned int	i;

	data->fork_mutex = malloc(sizeof(pthread_mutex_t) * data->n);
	if (!data->fork_mutex)
		return (false);
	i = 0;
	while (i < data->n)
	{
		if (pthread_mutex_init(&data->fork_mutex[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&data->fork_mutex[i]);
			}
			free(data->fork_mutex);
			data->fork_mutex = NULL;
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	init_philosopher_attributes(t_philo *ph, t_data *data,
	unsigned int i)
{
	ph->id = i + 1;
	ph->meals = 0;
	ph->last_meal = 0;
	ph->data = data;
	ph->left_fork = &data->fork_mutex[i];
	ph->right_fork = &data->fork_mutex[(i + 1) % data->n];
	if (pthread_mutex_init(&ph->meal_lock, NULL) != 0)
		return (false);
	return (true);
}

static bool	init_philos(t_data *data)
{
	unsigned int	i;

	data->philos = malloc(sizeof(t_philo) * data->n);
	if (!data->philos)
		return (false);
	i = 0;
	while (i < data->n)
	{
		if (!init_philosopher_attributes(&data->philos[i], data, i))
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&data->philos[i].meal_lock);
			}
			free(data->philos);
			data->philos = NULL;
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_data(t_data *data)
{
	data->dead = false;
	data->start_time = 0;
	data->fork_mutex = NULL;
	data->philos = NULL;
	if (!init_mutexes(data))
		return (false);
	if (!init_fork_mutexes(data))
		return (false);
	if (!init_philos(data))
		return (false);
	return (true);
}