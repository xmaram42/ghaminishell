NAME            := cub3D
CC              := cc
CFLAGS          := -Wall -Wextra -Werror

# ======================
# MiniLibX
# ======================
MLX_DIR         := mlx
MLX_LIB         := $(MLX_DIR)/libmlx.a
MLX_LDFLAGS     := -Lmlx -lmlx -framework OpenGL -framework AppKit

# ======================
# get_next_line
# ======================
GNL_DIR         := get_next_line
GNL_SRCS        := $(GNL_DIR)/get_next_line.c \
                   $(GNL_DIR)/get_next_line_utils.c\

# ======================
# Libft
# ======================
LIBFT_DIR       := libft
LIBFT           := $(LIBFT_DIR)/libft.a

# ======================
# Sources
# ======================
SRCS            := main.c helper.c \
					parsing/parse.c \
					parsing/elements.c \
					parsing/elements_utils.c \
					parsing/elements_utils1.c \
					parsing/map.c \
					parsing/validate_map.c \
					execution/init_game.c \
					execution/init_player.c \
					execution/render.c \
                   $(GNL_SRCS)

OBJS            := $(SRCS:.c=.o)

# ======================
# Include paths
# ======================
INC             := -I. -Ilibft -Iget_next_line -Imlx

# ======================
# Rules
# ======================
all: $(NAME)

$(NAME): $(MLX_LIB) $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(MLX_DIR) clean
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
