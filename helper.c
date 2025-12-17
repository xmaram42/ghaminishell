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
    free(parser->tex_no);
    free(parser->tex_so);
    free(parser->tex_we);
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