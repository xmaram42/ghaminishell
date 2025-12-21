/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:27:08 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 14:46:09 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


int error_msg(char *msg)
{
    printf("Error\n%s\n", msg);
    return (1);
}

int free_textures(t_parse *parser)
{
    if (parser->tex_no)
        free(parser->tex_no);
    if (parser->tex_so)
        free(parser->tex_so);
    if (parser->tex_we)
        free(parser->tex_we);
    if (parser->tex_ea)
        free(parser->tex_ea);

    parser->tex_no = NULL;
    parser->tex_so = NULL;
    parser->tex_we = NULL;
    parser->tex_ea = NULL;

    return (1);
}

int free_map(char **to_free, char *msg)
{
    int i = 0;

    if (to_free)
    {
        while (to_free[i])
        {
            free(to_free[i]);
            i++;
        }
        free(to_free);
    }
    return (error_msg(msg));
}

void cleanup_parser(t_parse *parser)
{
    if (parser->tex_no)
        free(parser->tex_no);
    if (parser->tex_so)
        free(parser->tex_so);
    if (parser->tex_we)
        free(parser->tex_we);
    if (parser->tex_ea)
        free(parser->tex_ea);
    if (parser->map.map)
    {
        int i = 0;
        while (parser->map.map[i])
        {
            free(parser->map.map[i]);
            i++;
        }
        free(parser->map.map);
    }
}

void cleanup_game(t_game *game)
{
    int i;

    if (!game)
        return;

    // Free textures
    if (game->mlx)
    {
        if (game->tex_north.img)
            mlx_destroy_image(game->mlx, game->tex_north.img);
        if (game->tex_south.img)
            mlx_destroy_image(game->mlx, game->tex_south.img);
        if (game->tex_east.img)
            mlx_destroy_image(game->mlx, game->tex_east.img);
        if (game->tex_west.img)
            mlx_destroy_image(game->mlx, game->tex_west.img);
        if (game->window)
            mlx_destroy_window(game->mlx, game->window);
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }

    // Free texture paths
    if (game->tex_no_path)
        free(game->tex_no_path);
    if (game->tex_so_path)
        free(game->tex_so_path);
    if (game->tex_ea_path)
        free(game->tex_ea_path);
    if (game->tex_we_path)
        free(game->tex_we_path);

    // Free map
    if (game->map.map)
    {
        i = 0;
        while (game->map.map[i])
        {
            free(game->map.map[i]);
            i++;
        }
        free(game->map.map);
    }
}
