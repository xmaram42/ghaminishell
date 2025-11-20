/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 14:21:22 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/20 17:39:00 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
/* ===================== INCLUDES ===================== */
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
# include <termios.h>

/* ===================== GLOBAL VARIABLE ===================== */
extern pid_t	g_signal_pid;

/* ===================== TOKEN TYPES ===================== */
# define TOK_CMD               0
# define TOK_ARG               1
# define TOK_PIPE              2
# define TOK_REDIR_IN  3
# define TOK_REDIR_OUT 4
# define TOK_APPEND    5
# define TOK_HEREDOC   6
# define TOK_PAREN_OPEN        7
# define TOK_PAREN_CLOSE       8
# define TOK_SEMICOLON 9
# define TOK_AND              10
/* ===================== QUOTE MARKERS ===================== */
# define SQ_MARKER '\1'
# define DQ_MARKER '\2'
/* ===================== ERROR HANDLING ===================== */
typedef enum e_error_type
{
	ERR_CMD_NOT_FOUND,
	ERR_SYNTAX,
	ERR_UNCLOSED_QUOTE,
	ERR_NUMERIC_ARG,
	ERR_TOO_MANY_ARGS,
	ERR_INVALID_IDENTIFIER,
	ERR_HOME_NOT_SET,
	ERR_OLDPWD_NOT_SET,
	ERR_ENV_NOT_SET,
	ERR_NO_SUCH_FILE,
	ERR_PERMISSION,
	ERR_IS_DIRECTORY,
	ERR_AMBIGUOUS_REDIRECT,
	ERR_HEREDOC_EOF,
	ERR_FORK,
	ERR_PIPE,
	ERR_OPEN_FILE,
	ERR_CHDIR,
	ERR_EXECVE,
	ERR_WAIT,
	ERR_ENV_INIT,
	ERR_ALLOCATION,
	ERR_GENERAL
}			t_error_type;
/* ===================== DATA STRUCTURES ===================== */

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_list
{
	char			*str;
	struct s_list	*prev;
	struct s_list	*next;
}	t_list;

typedef struct s_cmd
{
	bool			skip_cmd;
	char			**argv;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_data
{
	t_list			*env;
	t_cmd			*cmds;
	t_token			*token;
	int				exit_code;
	int				pip[2];
	bool			sq;
	bool			exit_flag;
}	t_data;

typedef struct s_syntax_state
{
	int	has_cmd;
	int	pending_redir;
}		t_syntax_state;

typedef struct s_expand
{
	char	*result;
	int		index;
	bool	in_single;
	int		last_exit;
	char	**envp;
}	t_expand;

/* ===================== SHELL STATE (LEGACY) ===================== */
typedef struct s_shell_state
{
	int				last_status;
	t_list			**env;
	t_token			*token;
	pid_t			active_child;
}	t_shell_state;

/* ===================== UTILITY FUNCTIONS ===================== */

// List utilities
int		append_to_list(t_list **list, char *s);
int		list_length(t_list *list);
void	free_env_list(t_list **list);
char	**lst_to_arr(t_list *env);
void	sort_array(char **arr, int len);

// Array utilities
void	free_array(char **array);
void	skip_spaces(char **str);
char	*extract_arg(char *start, char *end);
void	error_type_msg(t_error_type type, char *subject, char *detail,
			int errnum);

// Signal handling
void	setup_parent_signals(void);
void	setup_child_signals(void);
void	signals2(void);
void	disable_echoctl(void);

// String utilities
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin_free(char *s1, char *s2);

/* ===================== PARSING FUNCTIONS ===================== */

// Lexer
t_token	*lexer(char *line);
t_token	*new_token(char *str, int type);
void	add_token_back(t_token **lst, t_token *new);
int		handle_word(t_token **head, char *line, int i);
int		get_quoted_str(char *line, int i, char **out);
int		append_part(char **word, char *part);
int		has_unclosed_quote(const char *line);
int		validate_tokens(t_token *tokens, t_data *data);
int		process_token(t_token *tok, t_data *data, t_syntax_state *state);
void	store_word(t_syntax_state *state);
int		handle_redir(t_token *tok, t_data *data, t_syntax_state *state);
int		handle_pipe(t_token *tok, t_data *data, int has_cmd,
			int pending_redir);
int	handle_pipe_token(t_token **head, char *line, int i);
int		is_double_pipe(t_token *tok);
int		token_is_redir(int type);
void	syntax_error(t_data *data, char *token);
int	handle_and_token(t_token **head, char *line, int i);
int	handle_redir_in(t_token **head, char *line, int i);

char	*wrap_with_marker(char *content, char marker);

bool	is_plain_break(char c);
bool	is_word_stop(char c);

// Parser
t_cmd	*parser(t_token *tokens, t_data *data);
t_cmd	*new_cmd(void);
void	add_cmd_back(t_cmd **lst, t_cmd *new);
void	pars_word(t_cmd *cmd, t_token *tok, t_data *data);
void	parse_redir(t_cmd *cmd, t_token *tok, t_data *data);
int		validate_tokens(t_token *tokens, t_data *data);

// Expander
char	*expand_value(char *word, char **envp, int last_exit);
char	*find_env_value(char *key, char **envp);
char	*extract_var_name(char *str, int start);
char	*expand_tilde(char *word, char **envp);
char	*ft_free_first_str(char *s1, char *s2);
char	*strip_markers(const char *word);

// Input handling
char	*read_full_line(void);

/* ===================== BUILTIN FUNCTIONS ===================== */

// Core builtins
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_env(char **envp);
int		ft_cd(char **args, t_list **env);
int		ft_export(char **str, t_list **env);
// int		exec_unset(char **argv, t_shell_state *state);
void	ft_exit(t_data *data, char **args);
int		ft_unset(char **argv, t_list **env);

// Builtin helpers
int		is_valid_env_var_name(char *name);
int		strings_equal(const char *a, const char *b);
char	*create_env_var_string(char *key, char *value);
int		handle_cd_with_path(char *path);
bool	export(char *str, t_list **env);

// Builtin wrappers for execution
int		builtin_cd(t_data *shell, t_cmd *command);
int		builtin_pwd(void);
int		builtin_export(t_data *shell, t_cmd *command);
int		builtin_unset(t_data *shell, t_cmd *command);
int		builtin_env(t_data *shell);
int		builtin_echo(t_cmd *command);
int		clear_builtin(void);
bool	launch_builtin(t_data *shell, t_cmd *command);
bool	is_builtin(char *cmd);
int		builtin_colon(t_cmd *command);

/* ===================== EXECUTION FUNCTIONS ===================== */

// Pipeline execution
bool	exec_pipeline(t_data *data);
void	child_process(t_data *data, t_cmd *cmd, int *pip);
char	*find_cmd(t_data *data, char *cmd);

// Heredoc
int		handle_heredoc(char *delimiter, bool expand, t_data *data);
// Shell execution flow
bool	shell_parse_line(t_data *data, char *line);
bool	shell_exec(t_data *data);
int		shell_step(t_data *data);
void	shell_teardown(t_data *data);
void	shell_cleanup(t_data *data);

// Exit status management
void	ms_set_exit_status(t_data *data, int status);
int		ms_get_exit_status(t_data *data);

/* ===================== MEMORY MANAGEMENT ===================== */
void	free_token(t_token **list);
void	free_cmds(t_cmd **cmds);

/* ===================== ERROR MSGS MANAGEMENT ===================== */
void	err_prefix(void);
void	err_prefix_subject(char *subject);
void	err_cmd_not_found(char *cmd);
void	err_unexpected_token(char *token);
void	err_unclosed_quote(void);
void	err_with_errno(char *subject, char *detail, int errnum);
void	err_simple_subject(char *subject, char *message);
void	err_invalid_identifier(char *subject, char *identifier);
void	err_too_many(char *subject);
void	err_numeric(char *subject, char *detail);
int		handle_state_errors(t_error_type type, char *subject,
			char *detail);
void	error_type_msg(t_error_type type, char *subject, char *detail,
			int errnum);
void	err_heredoc(char *delimiter);
void	err_allocation(void);
void	err_env_init(void);
void	dispatch_system_errors(t_error_type type, char *subject,
			char *detail, int errnum);
int		handle_primary_errors(t_error_type type, char *subject, char *detail);
int		handle_cd_to_home(t_list *env);
int		cd_change_directory(char **args, t_list **env, char *old_pwd);
char	*get_home_from_env_list(t_list *env);

#endif