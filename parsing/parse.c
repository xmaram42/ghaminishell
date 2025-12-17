/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 13:02:46 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 16:15:53 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


int check_args(int ac, char **av)
{
    if (ac != 2)
   {
    printf("Error\nWrite *.cub file only\n");
    return (1);
   }
   
   if (ft_strncmp(av[1] + (ft_strlen(av[1]) - 4), ".cub", 4) != 0)
   {
    printf("Error\nWrite *.cub file only\n");
    return (1);
   }
   return (0);
}

int parse(char *av, t_parse *parser)
{
    int fd;
    
    fd = open(av, O_RDONLY);
    if(fd < 0)
        return (error_msg("File not found"));
    if(parse_elements(fd, parser) == 1)
    {
        close(fd);
        return (1);
    } 
    if(parse_map(&av[1], parser) == 1)
    {
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}
