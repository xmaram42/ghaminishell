/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 11:48:38 by maabdulr          #+#    #+#             */
/*   Updated: 2025/12/17 15:26:38 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

# include "get_next_line/get_next_line.h"
# include "libft/libft.h"
# include "mlx/mlx.h"
# include <fcntl.h>
# include <limits.h>
# include <stdio.h> 
# include <stdlib.h>
# include <unistd.h>

#define WIN_W 1024
#define WIN_H 768

#define EL_NO 0
#define EL_SO 1
#define EL_WE 2
#define EL_EA 3
#define EL_F  4
#define EL_C  5

typedef struct s_el_count
{
    int no;
    int so;
    int we;
    int ea;
    int f;
    int c;
}   t_el_count;

typedef struct s_map
{
    char    **map;
    int     w;
    int     h;
}       t_map;

typedef struct s_parse
{
    t_el_count  el;
    t_map       map;

    char        *tex_no;
    char        *tex_so;
    char        *tex_we;
    char        *tex_ea;

    int         floor_color;
    int         ceiling_color;
} t_parse;


typedef struct s_player
{
    float   x;
    float   y;
    float   dir_x;
    float   dir_y;
    float   plane_x;
    float   plane_y;
}               t_player;

typedef struct s_game
{
    t_map       map;
    t_player    player;

    void        *mlx;
    void        *window;

    void        *img;
    char        *img_addr;
    int         bpp;
    int         line_len;
    int         endian;

    int         floor_color;
    int         ceiling_color;

}   t_game;


// heper
int error_msg(char *msg);
int free_textures(t_parse *parser);
int free_map(char **to_free, char *msg);


//Parser
int check_args(int ac, char **av);
int parse(char *av, t_parse *parser);

//elements
int parse_elements(int fd, t_parse *parser);
int is_empty_line(char *line);
int all_elements_present(t_el_count *el);
void init_el(t_el_count *el);

// elements_utils
int is_duplicate(int type, t_el_count *el);
int parse_texture(char *value);
int process_element(int type, t_el_count *el);
int get_element_type(char *line, int i);
int process_line(char *line, t_parse *parser);

// elements_utils1
int parse_rgb_number(char *s, int *i, int *value);
int parse_color(char *value, int *out_color);

// map
int parse_map(char *path, t_parse *parser);
int store_map(int fd, t_parse *parser);
int count_map(int fd, t_parse *parser);
char *skip_to_map(int fd);
int line_len(char *line);

//validate_map
int validate_map(char **map);
int check_allowed_chars(char **map);
int check_player(char **map);
int check_map_closed(char **map);
int is_invalid_neighbor(char **map, int row, int col);

// init_game
int init_game(t_game *game, t_parse *parser);
int init_mlx(t_game *game);

// init_player
void init_player(t_parse *parser, t_player *player);

//render
int draw_floor_ceiling(t_game *game);

#endif