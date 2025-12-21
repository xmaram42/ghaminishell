/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 19:33:26 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 18:27:52 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int is_duplicate(int type, t_el_count *el)
{
    if (type == EL_NO && el->no != -1)
        return (error_msg("Duplicate NO"));
    if (type == EL_SO && el->so != -1)
        return (error_msg("Duplicate SO"));
    if (type == EL_WE && el->we != -1)
        return (error_msg("Duplicate WE"));
    if (type == EL_EA && el->ea != -1)
        return (error_msg("Duplicate EA"));
    if (type == EL_F && el->f != -1)
        return (error_msg("Duplicate F"));
    if (type == EL_C && el->c != -1)
        return (error_msg("Duplicate C"));
    return (0);
}

int parse_texture(char *value)
{
    int i;
    int start;
    int end;
    int fd;
    char temp;

    i = 0;
    while (value[i] == ' ')
        i++;

    if (value[i] == '\0')
        return (error_msg("Empty texture path"));

    start = i;

    while (value[i] && value[i] != ' ' && value[i] != '\n')
        i++;

    end = i;

    if (end - start < 5 || ft_strncmp(value + end - 4, ".xpm", 4) != 0)
        return (error_msg("Texture must be a .xpm file"));

    while (value[i] == ' ')
        i++;

    if (value[i] != '\0' && value[i] != '\n')
        return (error_msg("Invalid texture path"));

    temp = value[end];
    value[end] = '\0';
    fd = open(value + start, O_RDONLY);
    value[end] = temp;
    
    if (fd < 0)
        return (error_msg("Cannot open texture file"));

    close(fd);
    return (0);
}


int process_element(int type, t_el_count *el)
{
    if (is_duplicate(type, el))
        return (1);
    if (type == EL_NO)
        el->no = 1;
    else if (type == EL_SO)
        el->so = 1;
    else if (type == EL_WE)
        el->we = 1;
    else if (type == EL_EA)
        el->ea = 1;
    else if (type == EL_F)
        el->f = 1;
    else if (type == EL_C)
        el->c = 1;
    return (0);
}

int get_element_type(char *line, int i)
{
    if (!ft_strncmp(line + i, "NO ", 3))
        return (EL_NO);
    if (!ft_strncmp(line + i, "SO ", 3))
        return (EL_SO);
    if (!ft_strncmp(line + i, "WE ", 3))
        return (EL_WE);
    if (!ft_strncmp(line + i, "EA ", 3))
        return (EL_EA);
    if (!ft_strncmp(line + i, "F ", 2))
        return (EL_F);
    if (!ft_strncmp(line + i, "C ", 2))
        return (EL_C);
    return (-1);
}

int process_line(char *line, t_parse *parser)
{
    int i;
    int type;
    char *value;
    int j;

    i = 0;
    while (line[i] == ' ')
        i++;
    /* forbid tabs in element definitions */
    j = i;
    while (line[j] && line[j] != '\n')
    {
        if (line[j] == '\t')
            return (error_msg("Invalid element Tab character"));
        j++;
    }
    type = get_element_type(line, i);
    if (type == -1)
    {
        /* If line appears to start an element but is malformed, use one message */
        if (line[i] == 'N' || line[i] == 'S' || line[i] == 'W' ||
            line[i] == 'E' || line[i] == 'F' || line[i] == 'C')
            return (error_msg("Invalid element definition"));
        return (2);
    }
    if (process_element(type, &parser->el))
        return (1);
    if (type == EL_NO)
    {
        value = line + i + 3;
        if (parse_texture(value))
            return (1);
        parser->tex_no = extract_texture_path(value);
        if (!parser->tex_no)
            return (error_msg("Memory allocation failed"));
    }
    else if (type == EL_SO)
    {
        value = line + i + 3;
        if (parse_texture(value))
            return (1);
        parser->tex_so = extract_texture_path(value);
        if (!parser->tex_so)
            return (error_msg("Memory allocation failed"));
    }
    else if (type == EL_WE)
    {
        value = line + i + 3;
        if (parse_texture(value))
            return (1);
        parser->tex_we = extract_texture_path(value);
        if (!parser->tex_we)
            return (error_msg("Memory allocation failed"));
    }
    else if (type == EL_EA)
    {
        value = line + i + 3;
        if (parse_texture(value))
            return (1);
        parser->tex_ea = extract_texture_path(value);
        if (!parser->tex_ea)
            return (error_msg("Memory allocation failed"));
    }
    else if (type == EL_F)
    {
        value = line + i + 2;
        if (parse_color(value, &parser->floor_color))
            return (1);
    }
    else if (type == EL_C)
    {
        value = line + i + 2;
        if (parse_color(value, &parser->ceiling_color))
            return (1);
    }
    return (0);
}

