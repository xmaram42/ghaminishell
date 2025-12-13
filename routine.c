/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:14:19 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 11:28:45 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = (long long)get_time_ms();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	print_state(philo, EAT);
	usleep_ms(data->eat, data);
}

static void	sleep_philo(t_philo *philo)
{
	print_state(philo, SLEEP);
	usleep_ms(philo->data->sleep, philo->data);
}

static void	think_philo(t_philo *philo)
{
	print_state(philo, THINK);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->n == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, FORK);
		usleep_ms(philo->data->die, philo->data);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep_ms(10, philo->data);
	while (!simulation_stopped(philo->data))
	{
		take_forks(philo);
		eat(philo);
		put_forks(philo);
		sleep_philo(philo);
		think_philo(philo);
	}
	return (NULL);
}
