/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 12:58:41 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/16 19:58:52 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int line_len(char *line)
{
    int i = 0;

    while (line[i] && line[i] != '\n')
        i++;
    return (i);
}

char *skip_to_map(int fd)
{
    char *line;
    int i;

    line = get_next_line(fd);
    while (line)
    {
        i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (line[i] == '1')
            return (line);
        free(line);
        line = get_next_line(fd);
    }
    return (NULL);
}

int count_map(int fd, t_parse *parser)
{
    char *line;
    int   len;

    parser->map.h = 0;
    parser->map.w = 0;

    line = skip_to_map(fd);
    while (line)
    {
        parser->map.h++;
        len = line_len(line);
        if (len > parser->map.w)
            parser->map.w = len;
        free(line);
        line = get_next_line(fd);
    }
    return (0);
}

int store_map(int fd, t_parse *parser)
{
    char *line;
    int   i;

    parser->map.map = ft_calloc(parser->map.h + 1, sizeof(char *));
    if (!parser->map.map)
        return (error_msg("Memory allocation failed"));

    i = 0;
    line = skip_to_map(fd);
    while (line)
    {
        parser->map.map[i++] = line; // store as-is
        line = get_next_line(fd);
    }
    parser->map.map[i] = NULL;
    return (0);
}

int parse_map(char *path, t_parse *parser)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (error_msg("File not found"));
    count_map(fd, parser);
    close(fd);

    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (error_msg("File not found"));
    store_map(fd, parser);
    close(fd);
    if (validate_map(parser->map.map))
        return (1);
    return (0);
}
