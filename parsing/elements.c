/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:31:26 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 17:33:33 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void init_el(t_el_count *el)
{
    el->no = -1;
    el->so = -1;
    el->we = -1;
    el->ea = -1;
    el->f = -1;
    el->c = -1;
}

int all_elements_present(t_el_count *el)
{
    if (el->no != -1 &&
        el->so != -1 &&
        el->we != -1 &&
        el->ea != -1 &&
        el->f  != -1 &&
        el->c  != -1)
        return (1);
    return (0);
}

int is_empty_line(char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t')
            return (0);
        i++;
    }
    return (1);
}

int parse_elements(int fd, t_parse *parser)
{
    char    *line;
    int     ret;

    init_el(&parser->el);

    parser->tex_no = NULL;
    parser->tex_so = NULL;
    parser->tex_we = NULL;
    parser->tex_ea = NULL;

    line = get_next_line(fd);
    while (line)
    {
        if (is_empty_line(line))
        {
            free(line);
            line = get_next_line(fd);
            continue;
        }

        ret = process_line(line, parser);
        if (ret == 1)
        {
            free(line);
            return (free_textures(parser));
        }
        if (ret == 2)
        {
            free(line);
            if (!all_elements_present(&parser->el))
                return (free_textures(parser));
            break;
        }

        free(line);
        line = get_next_line(fd);
    }
    /* If EOF reached without a non-element line, ensure all elements exist */
    if (!all_elements_present(&parser->el))
        return (free_textures(parser));
    return (0);
}

