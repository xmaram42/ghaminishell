/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 11:48:25 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 15:24:57 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int ac, char **av)
{
    t_parse     parser;
    t_game      game;
    
    if (check_args(ac, av) == 1)
        return (1);
    if (parse(av[1], &parser))
        return (1);
    if(init_game(&game, &parser))
        return (1);
    if (init_mlx(&game))
        return (1);
    if (draw_floor_ceiling(&game))
        return (1);
    mlx_loop(game.mlx);
    return (0);
}