/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:43:04 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 18:33:03 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int load_texture(t_game *game, t_texture *tex, char *path)
{
    tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
    if (!tex->img)
    {
        printf("Error\nFailed to load texture: %s\n", path);
        return (1);
    }
    tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);
    if (!tex->addr)
    {
        mlx_destroy_image(game->mlx, tex->img);
        return (error_msg("Failed to get texture data"));
    }
    return (0);
}

int load_textures(t_game *game)
{
    game->tex_north.img = NULL;
    game->tex_south.img = NULL;
    game->tex_east.img = NULL;
    game->tex_west.img = NULL;

    if (load_texture(game, &game->tex_north, game->tex_no_path))
        return (1);
    if (load_texture(game, &game->tex_south, game->tex_so_path))
    {
        if (game->tex_north.img)
            mlx_destroy_image(game->mlx, game->tex_north.img);
        return (1);
    }
    if (load_texture(game, &game->tex_east, game->tex_ea_path))
    {
        if (game->tex_north.img)
            mlx_destroy_image(game->mlx, game->tex_north.img);
        if (game->tex_south.img)
            mlx_destroy_image(game->mlx, game->tex_south.img);
        return (1);
    }
    if (load_texture(game, &game->tex_west, game->tex_we_path))
    {
        if (game->tex_north.img)
            mlx_destroy_image(game->mlx, game->tex_north.img);
        if (game->tex_south.img)
            mlx_destroy_image(game->mlx, game->tex_south.img);
        if (game->tex_east.img)
            mlx_destroy_image(game->mlx, game->tex_east.img);
        return (1);
    }
    return (0);
}

int init_mlx(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (error_msg("mlx_init failed"));

    game->window = mlx_new_window(
        game->mlx,
        WIN_W,
        WIN_H,
        "cub3D"
    );
    if (!game->window)
        return (error_msg("mlx_new_window failed"));

    // Load textures after MLX is initialized
    if (load_textures(game))
        return (1);

    return (0);
}
 
int init_game(t_game *game, t_parse *parser)
{
    game->mlx = NULL;
    game->window = NULL;
    game->floor_color = parser->floor_color;
    game->ceiling_color = parser->ceiling_color;
    game->map = parser->map;

    // Store texture paths
    game->tex_no_path = parser->tex_no;
    game->tex_so_path = parser->tex_so;
    game->tex_ea_path = parser->tex_ea;
    game->tex_we_path = parser->tex_we;

    init_player(parser, &game->player);

    return (0);
}

