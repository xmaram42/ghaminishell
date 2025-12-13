NAME        = philo

SRC         = main.c \
			  init.c \
			  monitor.c\
			  philo.c \
			  parse.c \
			  utils.c \
			  routine.c \
			  fork.c\
			  time.c\
			  cleanup.c\


OBJ         = $(SRC:.c=.o)

CC          = cc
FLAGS       = -Wall -Wextra -Werror -pthread
DEL         = rm -rf


BLUE = \033[0;34m]


all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ)
 
%.o: %.c
	@echo "$(BLUE)Compiling $<..."
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@echo "$(BLUE)Cleaning object files..."
	@$(DEL) $(OBJ)

fclean: clean
	@echo "$(BLUE)Cleaning executable..."
	@$(DEL) $(NAME)

re: fclean all

.PHONY: all clean fclean re
