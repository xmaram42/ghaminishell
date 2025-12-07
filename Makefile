NAME        := minishell

CC          := cc
CFLAGS      := -Wall -Wextra -Werror

SRCS := srcs/main.c \
		srcs/exec/builtins_helpers.c \
		srcs/exec/builtins_launcher.c \
		srcs/exec/launcher_utils.c \
		srcs/exec/builtins_wrappers.c \
		srcs/exec/cmd_finder.c \
		srcs/exec/child/child.c \
		srcs/exec/child/child_utils.c \
		srcs/exec/child/child_utils1.c \
		srcs/exec/pipeline/pipeline.c \
		srcs/exec/pipeline/pipeline_utils.c \
		srcs/exec/pipeline/pipeline_utils1.c \
		srcs/exec/heredoc/heredoc.c\
		srcs/exec/heredoc/heredoc_utils.c\
		srcs/exec/heredoc/heredoc_utils1.c\
		srcs/exec/heredoc/heredoc_utils2.c\
		srcs/exec/shell_loop.c \
		srcs/exec/shell_exec.c \
        srcs/utils/list_token.c \
		srcs/utils/error_msg.c \
		srcs/utils/error_msg_1.c \
		srcs/utils/error_msg_2.c \
		srcs/utils/error_msg_3.c \
		srcs/utils/utils.c \
		srcs/utils/utils_array.c \
        srcs/utils/signals.c \
        srcs/utils/signals2.c \
        srcs/utils/free.c \
        srcs/utils/list_utils.c \
		srcs/parsing/lexer.c \
		srcs/parsing/lexers_utils.c \
		srcs/parsing/lexer_utils2.c \
		srcs/parsing/parser_utils.c \
		srcs/parsing/parser_utils_2.c \
		srcs/parsing/parser.c \
		srcs/parsing/cmd_fd.c \
		srcs/parsing/cmd_fd_2.c \
		srcs/parsing/lexer_quotes.c \
		srcs/parsing/lexer_quotes_2.c \
		srcs/parsing/readline_utils.c \
		srcs/parsing/expander.c \
		srcs/parsing/expand_loop.c \
		srcs/parsing/expanderr.c \
		srcs/parsing/expande_tilde.c \
		srcs/parsing/expander_utils.c \
		srcs/parsing/syntax_check.c \
		srcs/parsing/syntax_check_1.c \
		srcs/builtins/echo.c \
		srcs/builtins/pwd.c \
		srcs/builtins/exit.c \
		srcs/builtins/env.c \
		srcs/builtins/unset.c \
 		srcs/builtins/cd/cd.c \
		srcs/builtins/cd/cd_utils.c \
		srcs/builtins/cd/cd_utils1.c \
 		srcs/builtins/args_parse.c \
 		srcs/builtins/export/export.c \
		srcs/builtins/export/export_utils.c \
		srcs/builtins/export/export_utils1.c\
 		srcs/builtins/builtins_utils.c \

OBJS := $(SRCS:.c=.o)

# --- Libft ---
LIBFT_DIR   := ./libft
LIBFT       := $(LIBFT_DIR)/libft.a
LIBFT_INC   := -I$(LIBFT_DIR)

# --- Readline (macOS Homebrew) ---
READLINE_INC := 
READLINE_LIB := -lreadline

INCS        := -I. $(LIBFT_INC) $(READLINE_INC)

# --- Rules ---
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE_LIB) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
