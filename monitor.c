/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:17:37 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 11:33:22 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	all_philos_full(t_data *data)
{
	unsigned int	i;
	bool			full;

	if (data->n_eat == -1)
		return (false);
	i = 0;
	full = true;
	while (i < data->n)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		if (data->philos[i].meals < data->n_eat)
			full = false;
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	return (full);
}

static bool	check_philosopher_death(t_data *data)
{
	unsigned int		i;
	unsigned long long	now;
	unsigned long long	diff;

	now = get_time_ms();
	i = 0;
	while (i < data->n)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		diff = now - (unsigned long long)data->philos[i].last_meal;
		if (diff > data->die)
		{
			pthread_mutex_unlock(&data->philos[i].meal_lock);
			pthread_mutex_lock(&data->die_mutex);
			if (!data->dead)
				data->dead = true;
			pthread_mutex_unlock(&data->die_mutex);
			print_state(&data->philos[i], DEAD);
			return (true);
		}
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	return (false);
}

void	monitor_philos(t_data *data)
{
	while (!simulation_stopped(data))
	{
		if (check_philosopher_death(data))
			return ;
		if (all_philos_full(data))
		{
			pthread_mutex_lock(&data->die_mutex);
			data->dead = true;
			pthread_mutex_unlock(&data->die_mutex);
			return ;
		}
		usleep(1000);
	}
}

bool	simulation_stopped(t_data *data)
{
	bool	stopped;

	pthread_mutex_lock(&data->die_mutex);
	stopped = data->dead;
	pthread_mutex_unlock(&data->die_mutex);
	return (stopped);
}

void	print_state(t_philo *philo, int state)
{
	unsigned long long	now;
	unsigned long long	elapsed;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (simulation_stopped(philo->data) && state != DEAD)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	now = get_time_ms();
	elapsed = now - (unsigned long long)philo->data->start_time;
	if (state == EAT)
		printf("%llu %d is eating\n", elapsed, philo->id);
	else if (state == SLEEP)
		printf("%llu %d is sleeping\n", elapsed, philo->id);
	else if (state == THINK)
		printf("%llu %d is thinking\n", elapsed, philo->id);
	else if (state == FORK)
		printf("%llu %d has taken a fork\n", elapsed, philo->id);
	else if (state == DEAD)
		printf("%llu %d died\n", elapsed, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
