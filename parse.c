/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 14:30:38 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/10 15:37:52 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	arg_count_error(void)
{
	printf("Error: Invalid number of arguments.\n");
	return (false);
}

static bool	arg_value_error(char *arg)
{
	printf("Error: Invalid argument '%s'\n", arg);
	return (false);
}

static bool	check_empty(char *arg)
{
	if (arg[0] == '\0')
		return (arg_value_error(arg));
	return (true);
}

bool	validate_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac != 5 && ac != 6)
		return (arg_count_error());
	i = 1;
	while (i < ac)
	{
		if (!check_empty(av[i]))
			return (false);
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (arg_value_error(av[i]));
			j++;
		}
		i++;
	}
	return (true);
}

bool	parse_args(t_data *data, int argc, char **argv)
{
	data->n = (unsigned int)ft_atoi(argv[1]);
	data->die = (unsigned long long)ft_atoi(argv[2]);
	data->eat = (unsigned long long)ft_atoi(argv[3]);
	data->sleep = (unsigned long long)ft_atoi(argv[4]);
	if (argc == 6)
		data->n_eat = (long long)ft_atoi(argv[5]);
	else
		data->n_eat = -1;
	if (data->n == 0 || data->die == 0 || data->eat == 0 || data->sleep == 0)
	{
		printf("Error: Arguments must be greater than zero.\n");
		return (false);
	}
	if (argc == 6 && data->n_eat == 0)
	{
		printf("Error: Meals must be greater than zero.\n");
		return (false);
	}
	return (true);
}
