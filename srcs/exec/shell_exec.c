#include "minishell.h"
static bool     print_syntax_error(t_data *data, const char *token)
{
        ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
        if (token)
                write(2, token, ft_strlen(token));
        ft_putstr_fd("'\n", 2);
        data->exit_code = 2;
        return (true);
}

static bool     pipe_has_syntax_error(t_token *tokens, t_data *data)
{
        t_token *prev;

        prev = NULL;
        while (tokens)
        {
                if (tokens->type == TOK_PIPE)
                {
                        if (!prev || prev->type == TOK_PIPE)
                                return (print_syntax_error(data, tokens->str));
                        if (!tokens->next)
                                return (print_syntax_error(data, "newline"));
                        if (tokens->next->type == TOK_PIPE)
                                return (print_syntax_error(data, tokens->next->str));
                }
                prev = tokens;
                tokens = tokens->next;
        }
        return (false);
}

bool    shell_parse_line(t_data *data, char *line)
{
        t_token *tokens;

        if (has_unclosed_quote(line))
        {
                ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
                data->exit_code = 2;
                return (false);
        }
        tokens = lexer(line);
        if (!tokens)
                return (false);
        if (pipe_has_syntax_error(tokens, data))
        {
                free_token(&tokens);
                return (false);
        }
        data->token = tokens;
        data->cmds = parser(tokens, data);
        if (!data->cmds)
        {
                free_token(&data->token);
                data->token = NULL;
                return (false);
        }
        return (true);
}

bool    shell_exec(t_data *data)
{
        if (!data->cmds)
                return (false);
        return (exec_pipeline(data));
}

void    shell_cleanup(t_data *data)
{
        (void)data;
}