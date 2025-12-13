/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 11:16:23 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 11:30:40 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, FORK);
	}
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
