/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:44:34 by aalbugar          #+#    #+#             */
/*   Updated: 2025/12/11 11:41:03 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main (int argc, char **argv)
{
	t_data		data;

	if (!validate_args(argc, argv)|| !parse_args(&data, argc, argv))
		return (1);
	if (!init_data(&data))
		return (1);
	if (!start_philo(&data))
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}
