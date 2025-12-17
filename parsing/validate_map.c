/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:03:48 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/16 16:54:35 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int check_allowed_chars(char **map)
{
    int i;
    int j;
    char c;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            c = map[i][j];
            if (c == '\n')
            {
                j++;
                continue;
            }
            if (c != '0' && c != '1' && c != ' ' && c != 'N' && c != 'S'
                && c != 'E' && c != 'W')
                return (free_map(map, "Invalid character in map"));
            j++;
        }
        i++;
    }
    return (0);
}

int check_player(char **map)
{
    int i;
    int j;
    int player_count;
    char c;

    player_count = 0;
    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            c = map[i][j];
            if (c == '\n')
            {
                j++;
                continue;
            }
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
                player_count++;
            j++;
        }
        i++;
    }
    if (player_count != 1)
        return (free_map(map, "Invalid number of player start positions"));
    return (0);
}

int is_invalid_neighbor(char **map, int row, int col)
{
    if (row < 0)
        return (1);
    if (!map[row])
        return (1);
    if (col < 0)
        return (1);
    if (col >= line_len(map[row]))
        return (1);
    if (map[row][col] == ' ')
        return (1);
    return (0);
}

int check_map_closed(char **map)
{
    int i;
    int j;
    char c;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            c = map[i][j];
            if (c == '\n')
            {
                j++;
                continue;
            }
            if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                if (is_invalid_neighbor(map, i - 1, j) ||
                    is_invalid_neighbor(map, i + 1, j) ||
                    is_invalid_neighbor(map, i, j - 1) ||
                    is_invalid_neighbor(map, i, j + 1))
                    return (free_map(map, "Map is not closed"));
            }
            j++;
        }
        i++;
    }
    return (0);
}

int validate_map(char **map)
{
    if (check_allowed_chars(map))
        return (1);

    if (check_player(map))
        return (1);

    if (check_map_closed(map))
        return (1);

    return (0);
}