/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:44:37 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 14:36:12 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


int parse_rgb_number(char *s, int *i, int *value)
{
    int num;

    num = 0;
    if (!ft_isdigit(s[*i]))
        return (error_msg("Invalid color format"));

    while (ft_isdigit(s[*i]))
    {
        num = num * 10 + (s[*i] - '0');
        (*i)++;
    }
    if (num < 0 || num > 255)
        return (error_msg("Color value out of range"));
    *value = num;
    return (0);
}

int parse_color(char *value, int *out_color)
{
    int i;
    int r;
    int g;
    int b;

    i = 0;
    while (value[i] == ' ')
        i++;

    if (value[i] == '\0')
        return (error_msg("Empty color value"));

    if (parse_rgb_number(value, &i, &r))
        return (1);
    if (value[i++] != ',')
        return (error_msg("Invalid color format"));

    if (parse_rgb_number(value, &i, &g))
        return (1);
    if (value[i++] != ',')
        return (error_msg("Invalid color format"));

    if (parse_rgb_number(value, &i, &b))
        return (1);

    while (value[i] == ' ')
        i++;
    if (value[i] != '\0' && value[i] != '\n')
        return (error_msg("Invalid color format"));
    *out_color = (r << 16) | (g << 8) | b;
    return (0);
}
