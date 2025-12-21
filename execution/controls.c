/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/20 00:00:00 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int can_move(t_game *game, float new_x, float new_y)
{
    int map_x;
    int map_y;

    map_x = (int)new_x;
    map_y = (int)new_y;

    if (map_y < 0 || map_y >= game->map.h)
        return (0);
    if (map_x < 0 || map_x >= (int)line_len(game->map.map[map_y]))
        return (0);
    if (game->map.map[map_y][map_x] == '1')
        return (0);
    return (1);
}

static void move_forward(t_game *game)
{
    float new_x;
    float new_y;

    new_x = game->player.x + game->player.dir_x * MOVE_SPEED;
    new_y = game->player.y + game->player.dir_y * MOVE_SPEED;

    if (can_move(game, new_x, game->player.y))
        game->player.x = new_x;
    if (can_move(game, game->player.x, new_y))
        game->player.y = new_y;
}

static void move_backward(t_game *game)
{
    float new_x;
    float new_y;

    new_x = game->player.x - game->player.dir_x * MOVE_SPEED;
    new_y = game->player.y - game->player.dir_y * MOVE_SPEED;

    if (can_move(game, new_x, game->player.y))
        game->player.x = new_x;
    if (can_move(game, game->player.x, new_y))
        game->player.y = new_y;
}

static void move_left(t_game *game)
{
    float new_x;
    float new_y;

    new_x = game->player.x - game->player.plane_x * MOVE_SPEED;
    new_y = game->player.y - game->player.plane_y * MOVE_SPEED;

    if (can_move(game, new_x, game->player.y))
        game->player.x = new_x;
    if (can_move(game, game->player.x, new_y))
        game->player.y = new_y;
}

static void move_right(t_game *game)
{
    float new_x;
    float new_y;

    new_x = game->player.x + game->player.plane_x * MOVE_SPEED;
    new_y = game->player.y + game->player.plane_y * MOVE_SPEED;

    if (can_move(game, new_x, game->player.y))
        game->player.x = new_x;
    if (can_move(game, game->player.x, new_y))
        game->player.y = new_y;
}

static void rotate_left(t_game *game)
{
    float old_dir_x;
    float old_plane_x;

    old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(ROT_SPEED)
        - game->player.dir_y * sin(ROT_SPEED);
    game->player.dir_y = old_dir_x * sin(ROT_SPEED)
        + game->player.dir_y * cos(ROT_SPEED);

    old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(ROT_SPEED)
        - game->player.plane_y * sin(ROT_SPEED);
    game->player.plane_y = old_plane_x * sin(ROT_SPEED)
        + game->player.plane_y * cos(ROT_SPEED);
}

static void rotate_right(t_game *game)
{
    float old_dir_x;
    float old_plane_x;

    old_dir_x = game->player.dir_x;
    game->player.dir_x = game->player.dir_x * cos(-ROT_SPEED)
        - game->player.dir_y * sin(-ROT_SPEED);
    game->player.dir_y = old_dir_x * sin(-ROT_SPEED)
        + game->player.dir_y * cos(-ROT_SPEED);

    old_plane_x = game->player.plane_x;
    game->player.plane_x = game->player.plane_x * cos(-ROT_SPEED)
        - game->player.plane_y * sin(-ROT_SPEED);
    game->player.plane_y = old_plane_x * sin(-ROT_SPEED)
        + game->player.plane_y * cos(-ROT_SPEED);
}

int key_press(int keycode, t_game *game)
{
    if (keycode == KEY_ESC)
        close_window(game);
    else if (keycode == KEY_W)
        move_forward(game);
    else if (keycode == KEY_S)
        move_backward(game);
    else if (keycode == KEY_A)
        move_left(game);
    else if (keycode == KEY_D)
        move_right(game);
    else if (keycode == KEY_LEFT)
        rotate_left(game);
    else if (keycode == KEY_RIGHT)
        rotate_right(game);
    return (0);
}

int close_window(t_game *game)
{
    cleanup_game(game);
    exit(0);
    return (0);
}
