/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:43:04 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/16 20:12:17 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int init_mlx(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (error_msg("mlx_init failed"));

    game->window = mlx_new_window(
        game->mlx,
        1024,
        768,
        "cub3D"
    );
    if (!game->window)
        return (error_msg("mlx_new_window failed"));

    return (0);
}
 
int init_game(t_game *game, t_parse *parser)
{
    game->mlx = NULL;
    game->window = NULL;

    game->map = parser->map;

    init_player(parser, &game->player);

    return (0);
}

