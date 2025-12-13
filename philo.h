/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:45:30 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 13:18:21 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

# define EAT   0
# define SLEEP 1
# define THINK 2
# define FORK  3
# define DEAD  4

typedef struct s_philo	t_philo;

typedef struct s_data
{
	unsigned int		n;				// Number of philosophers
	unsigned long long	die;			// Time to die (ms)
	unsigned long long	eat;			// Time to eat (ms)
	unsigned long long	sleep;			// Time to sleep (ms)
	long long			start_time;		// Simulation start time (ms)
	long long			n_eat;			// Meals required (or -1)
	bool				dead;			// True if any philosopher died
	pthread_t			monitor;		// Monitor thread
	pthread_mutex_t		*fork_mutex;	// Array of fork mutexes
	pthread_mutex_t		print_mutex;	// Protects print output
	pthread_mutex_t		eat_mutex;		// Protects meal count updates
	pthread_mutex_t		die_mutex;		// Protects death flag
	t_philo				*philos;		// Array of philosopher structs
}	t_data;

struct s_philo
{
	int				id;				//Philosopher’s unique ID (1 → n)
	int				meals;			// Number of meals this philo has eaten
	long long		last_meal;		//Timestamp of the last time this philo ate
	pthread_t		thread;			// representing this philos life routine
	pthread_mutex_t	*left_fork;		//Pointer to the left fork mutex
	pthread_mutex_t	*right_fork;	//Pointer to the right fork mutex
	t_data			*data;			// Pointer to the shared simulation data
	pthread_mutex_t	meal_lock;		//Mutex to protect access 
};


bool				validate_args(int ac, char **av);
bool				parse_args(t_data *data, int argc, char **argv);
bool				init_data(t_data *data);
int					ft_atoi(const char *str);
int					usleep_ms(unsigned long long time, t_data *data);
bool				start_philo(t_data *data);
void				*philo_routine(void *arg);
bool				simulation_stopped(t_data *data);
void				take_forks(t_philo *philo);
void				put_forks(t_philo *philo);
void				monitor_philos(t_data *data);
void				print_state(t_philo *philo, int state);
void				cleanup(t_data *data);
unsigned long long	get_time_ms(void);



#endif
