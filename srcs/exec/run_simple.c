
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simple.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:31:14 by codespace         #+#    #+#             */
/*   Updated: 2025/09/16 10:01:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int compute_status_from_wait(int status)
{
    if (WIFEXITED(status)) //returns true if the child process ended normally
        return (WEXITSTATUS(status));//extracts the actual exit code
    if (WIFSIGNALED(status))//is true if the child process was killed by a signal.
        return (128 + WTERMSIG(status));//gives the signal number that killed the child.
    return (1); /* fallback */ // gneric error
}

int run_simple_command(char *const argv[], char *const envp[], t_shell_state *state)
{
    pid_t pid;
    int   status;

    pid = fork();
    if (pid < 0)
    {
        perror("lolipop🍭: fork");
        return (1);
    }
    if (pid == 0)
    {
        /* --- Child process --- */
        setup_child_signals();                     /* restore SIGINT/SIGQUIT defaults */
        execve(argv[0], (char *const *)argv, (char *const *)envp);
        /* If execve returns, it failed: choose 126 vs 127 by errno */
        perror("lolipop🍭: execve");
        _exit((errno == EACCES) ? 126 : 127);
    }
    state->active_child = pid;                     /* we’re executing a child now */
    if (waitpid(pid, &status, 0) < 0)
    {
        perror("lolipop🍭: waitpid");
        state->active_child = 0;
        return (1);
    }
    state->active_child = 0;                       /* back to prompt */
    return (compute_status_from_wait(status));     /* normalize to $? */
}
