/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 11:48:25 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 18:45:35 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int ac, char **av)
{
    t_parse     parser;
    t_game      game;
    
    ft_bzero(&parser, sizeof(t_parse));
    ft_bzero(&game, sizeof(t_game));
    
    if (check_args(ac, av) == 1)
        return (1);
    if (parse(av[1], &parser))
    {
        cleanup_parser(&parser);
        return (1);
    }
    if(init_game(&game, &parser))
    {
        cleanup_parser(&parser);
        return (1);
    }
    if (init_mlx(&game))
    {
        cleanup_game(&game);
        return (1);
    }
    
    // Set up hooks
    mlx_hook(game.window, 2, 1L<<0, key_press, &game);
    mlx_hook(game.window, 17, 0, close_window, &game);
    mlx_loop_hook(game.mlx, render_frame, &game);
    
    mlx_loop(game.mlx);
    
    cleanup_game(&game);
    return (0);
}
