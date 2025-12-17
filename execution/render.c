/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:17:19 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 15:25:08 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int draw_floor_ceiling(t_game *game)
{
    int x;
    int y;
    unsigned int color;
    char *dst;

    game->img = mlx_new_image(game->mlx, WIN_W, WIN_H);
    if (!game->img)
        return (error_msg("mlx_new_image failed"));

    game->img_addr = mlx_get_data_addr(
        game->img,
        &game->bpp,
        &game->line_len,
        &game->endian
    );
    if (!game->img_addr)
        return (error_msg("mlx_get_data_addr failed"));

    y = 0;
    while (y < WIN_H)
    {
        x = 0;
        while (x < WIN_W)
        {
            if (y < WIN_H / 2)
                color = game->ceiling_color;
            else
                color = game->floor_color;

            dst = game->img_addr
                + (y * game->line_len)
                + (x * (game->bpp / 8));

            *(unsigned int *)dst = color;
            x++;
        }
        y++;
    }

    mlx_put_image_to_window(game->mlx, game->window, game->img, 0, 0);
    mlx_destroy_image(game->mlx, game->img);

    return (0);
}
