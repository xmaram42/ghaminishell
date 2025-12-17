/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:24:51 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/16 21:55:43 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void set_north_south(t_player *p, char dir)
{
    if (dir == 'N')
    {
        p->dir_x = 0;
        p->dir_y = -1;
        p->plane_x = 0.66;
        p->plane_y = 0;
    }
    else if (dir == 'S')
    {
        p->dir_x = 0;
        p->dir_y = 1;
        p->plane_x = -0.66;
        p->plane_y = 0;
    }
}

static void set_east_west(t_player *p, char dir)
{
    if (dir == 'E')
    {
        p->dir_x = 1;
        p->dir_y = 0;
        p->plane_x = 0;
        p->plane_y = 0.66;
    }
    else if (dir == 'W')
    {
        p->dir_x = -1;
        p->dir_y = 0;
        p->plane_x = 0;
        p->plane_y = -0.66;
    }
}

static void set_player_direction(t_player *player, char dir)
{
    if (dir == 'N' || dir == 'S')
        set_north_south(player, dir);
    else if (dir == 'E' || dir == 'W')
        set_east_west(player, dir);
}

void init_player(t_parse *parser, t_player *player)
{
    int     y;
    int     x;
    char    c;

    y = 0;
    while (parser->map.map[y])
    {
        x = 0;
        while (parser->map.map[y][x])
        {
            c = parser->map.map[y][x];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                player->x = x + 0.5;
                player->y = y + 0.5;
                set_player_direction(player, c);
                parser->map.map[y][x] = '0';
                return ;
            }
            x++;
        }
        y++;
    }
}
