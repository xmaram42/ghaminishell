/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:17:19 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 20:07:29 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void render_floor_ceiling(t_game *game)
{
    int x;
    int y;
    unsigned int color;
    char *dst;

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
}

void init_ray(t_game *game, t_ray *ray, int x)
{
    ray->camera_x = 2.0 * x / (double)WIN_W - 1.0;
    ray->ray_dir_x = game->player.dir_x + game->player.plane_x * ray->camera_x;
    ray->ray_dir_y = game->player.dir_y + game->player.plane_y * ray->camera_x;

    ray->map_x = (int)game->player.x;
    ray->map_y = (int)game->player.y;

    if (ray->ray_dir_x == 0)
        ray->delta_dist_x = DBL_MAX;
    else
        ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);

    if (ray->ray_dir_y == 0)
        ray->delta_dist_y = DBL_MAX;
    else
        ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);

    if (ray->ray_dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_dist_x = (game->player.x - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1.0 - game->player.x) * ray->delta_dist_x;
    }

    if (ray->ray_dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1.0 - game->player.y) * ray->delta_dist_y;
    }

    ray->hit = 0;
    ray->side = 0;
}

int is_wall(t_game *game, int mx, int my)
{
    if (my < 0 || my >= game->map.h)
        return (1);
    if (mx < 0 || mx >= (int)line_len(game->map.map[my]))
        return (1);
    return (game->map.map[my][mx] == '1');
}

void perform_dda(t_game *game, t_ray *ray)
{
    while (ray->hit == 0)
    {
        if (ray->side_dist_x < ray->side_dist_y)
        {
            ray->side_dist_x += ray->delta_dist_x;
            ray->map_x += ray->step_x;
            ray->side = 0;
        }
        else
        {
            ray->side_dist_y += ray->delta_dist_y;
            ray->map_y += ray->step_y;
            ray->side = 1;
        }
        if (is_wall(game, ray->map_x, ray->map_y))
            ray->hit = 1;
    }
}

void calculate_wall(t_game *game, t_ray *ray)
{
    double  perp_wall_dist;
    int     line_height;
    int     draw_start;
    int     draw_end;

    if (ray->side == 0)
    {
        if (ray->ray_dir_x == 0)
            perp_wall_dist = DBL_MAX;
        else
            perp_wall_dist = (ray->map_x - game->player.x 
                + (1 - ray->step_x) / 2) / ray->ray_dir_x;
    }
    else
    {
        if (ray->ray_dir_y == 0)
            perp_wall_dist = DBL_MAX;
        else
            perp_wall_dist = (ray->map_y - game->player.y 
                + (1 - ray->step_y) / 2) / ray->ray_dir_y;
    }

    line_height = (int)(WIN_H / perp_wall_dist);
    draw_start = -line_height / 2 + WIN_H / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = line_height / 2 + WIN_H / 2;
    if (draw_end >= WIN_H)
        draw_end = WIN_H - 1;

    ray->draw_start = draw_start;
    ray->draw_end = draw_end;
    ray->line_height = line_height;
}

void draw_wall_stripe(t_game *game, t_ray *ray, int x)
{
    t_texture   *tex;
    double      wall_x;
    int         tex_x;
    int         y;
    int         tex_y;
    double      step;
    double      tex_pos;
    int         color;
    char        *dst;

    // Determine which texture to use based on ray direction
    if (ray->side == 0)  // Vertical wall (EW)
    {
        if (ray->ray_dir_x > 0)
            tex = &game->tex_east;
        else
            tex = &game->tex_west;
    }
    else  // Horizontal wall (NS)
    {
        if (ray->ray_dir_y > 0)
            tex = &game->tex_south;
        else
            tex = &game->tex_north;
    }

    // Calculate wall_x (where exactly the wall was hit)
    if (ray->side == 0)
        wall_x = game->player.y + ((ray->map_x - game->player.x + (1 - ray->step_x) / 2) / ray->ray_dir_x) * ray->ray_dir_y;
    else
        wall_x = game->player.x + ((ray->map_y - game->player.y + (1 - ray->step_y) / 2) / ray->ray_dir_y) * ray->ray_dir_x;
    wall_x -= floor(wall_x);

    // x coordinate on the texture
    tex_x = (int)(wall_x * (double)tex->width);
    if ((ray->side == 0 && ray->ray_dir_x < 0) || (ray->side == 1 && ray->ray_dir_y > 0))
        tex_x = tex->width - tex_x - 1;

    // Calculate texture step and starting position
    step = 1.0 * tex->height / ray->line_height;
    tex_pos = (ray->draw_start - WIN_H / 2 + ray->line_height / 2) * step;

    // Draw the vertical stripe
    y = ray->draw_start;
    while (y <= ray->draw_end)
    {
        tex_y = (int)tex_pos & (tex->height - 1);
        tex_pos += step;

        // Get color from texture
        color = *(int *)(tex->addr + (tex_y * tex->line_len + tex_x * (tex->bpp / 8)));

        // Put pixel to image
        dst = game->img_addr + (y * game->line_len + x * (game->bpp / 8));
        *(unsigned int *)dst = color;

        y++;
    }
}

void render_walls(t_game *game)
{
    t_ray   ray;
    int     x;

    x = 0;
    while (x < WIN_W)
    {
        init_ray(game, &ray, x);
        perform_dda(game, &ray);
        calculate_wall(game, &ray);
        draw_wall_stripe(game, &ray, x);
        x++;
    }
}

int render_frame(t_game *game)
{
    game->img = mlx_new_image(game->mlx, WIN_W, WIN_H);
    game->img_addr = mlx_get_data_addr(
        game->img,
        &game->bpp,
        &game->line_len,
        &game->endian
    );
    if (!game->img || !game->img_addr)
        return (1);

    render_floor_ceiling(game);
    render_walls(game);

    mlx_put_image_to_window(game->mlx, game->window, game->img, 0, 0);
    mlx_destroy_image(game->mlx, game->img);

    return (0);
}
