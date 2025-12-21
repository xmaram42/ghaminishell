NAME            := cub3D
CC              := cc
CFLAGS          := -Wall -Wextra -Werror

# ======================
# OS Detection
# ======================
UNAME_S         := $(shell uname -s)

# ======================
# MiniLibX Configuration (Auto-detect)
# ======================
ifeq ($(UNAME_S),Linux)
	# Linux configuration
	MLX_DIR     := mlx_linux
	MLX_LIB     := $(MLX_DIR)/libmlx.a
	MLX_LDFLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
	MLX_BUILD   := $(MAKE) -C $(MLX_DIR)
	PLATFORM    := Linux
else ifeq ($(UNAME_S),Darwin)
	# macOS configuration
	MLX_DIR     := mlx_mac
	MLX_LIB     := $(MLX_DIR)/libmlx.a
	MLX_LDFLAGS := -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	MLX_BUILD   := $(MAKE) -C $(MLX_DIR)
	PLATFORM    := macOS
else
	# Fallback to Linux
	MLX_DIR     := mlx_linux
	MLX_LIB     := $(MLX_DIR)/libmlx.a
	MLX_LDFLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
	MLX_BUILD   := $(MAKE) -C $(MLX_DIR)
	PLATFORM    := Unknown (defaulting to Linux)
endif

# ======================
# get_next_line
# ======================
GNL_DIR         := get_next_line
GNL_SRCS        := $(GNL_DIR)/get_next_line.c \
                   $(GNL_DIR)/get_next_line_utils.c

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
					execution/controls.c \
                   $(GNL_SRCS)

OBJS            := $(SRCS:.c=.o)

# ======================
# Include paths
# ======================
INC             := -I. -I$(LIBFT_DIR) -I$(GNL_DIR) -I$(MLX_DIR)

# ======================
# Rules
# ======================
all: banner $(NAME)

banner:
	@echo "=========================================="
	@echo "  Building Cub3D for $(PLATFORM)"
	@echo "=========================================="

$(NAME): $(MLX_LIB) $(LIBFT) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LDFLAGS) -o $(NAME)
	@echo "✓ Build complete for $(PLATFORM)!"

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

$(MLX_LIB):
	@echo "Building MiniLibX for $(PLATFORM)..."
	@$(MLX_BUILD) --no-print-directory

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS)
	@$(MAKE) -C $(MLX_DIR) clean --no-print-directory
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory
	@echo "✓ Clean complete"

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory
	@echo "✓ Full clean complete"

re: fclean all

.PHONY: all clean fclean re banner
