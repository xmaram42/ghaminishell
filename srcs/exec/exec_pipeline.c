/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/06 16:23:08 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    is_builtin(char *cmd)
{
    if (!cmd)
        return (false);
    if (!ft_strcmp(cmd, "echo")
        || !ft_strcmp(cmd, "cd")
        || !ft_strcmp(cmd, "pwd")
        || !ft_strcmp(cmd, "export")
        || !ft_strcmp(cmd, "unset")
        || !ft_strcmp(cmd, "env")
        || !ft_strcmp(cmd, "exit")
        || !ft_strcmp(cmd, "clear"))
        return (true);
    return (false);
}

static void parent_process(t_data *data, t_cmd *cmd, int *pip)
{
    (void)data;
    close(pip[1]);
    if (cmd->infile >= 0)
        close(cmd->infile);
    if (cmd->next && cmd->next->infile == -1)
        cmd->next->infile = pip[0];
    else
        close(pip[0]);
}

/* return child pid via out param */
static bool exec_cmd(t_data *data, t_cmd *cmd, int *pip, pid_t *out_pid)
{
    pid_t   pid;

    pid = fork();
    if (pid < 0)
    {
        perror("minishell: fork");
        return (false);
    }
    else if (pid == 0)
    {
        child_process(data, cmd, pip);
        exit(data->exit_code);
    }
    else
    {
        *out_pid = pid;
        parent_process(data, cmd, pip);
        return (true);
    }
}

static void wait_all(t_data *data, pid_t last_pid)
{
    int     status;
    pid_t   pid;
    int     last_status;

    last_status = 0;
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if (pid == last_pid)
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
    }
    data->exit_code = last_status;
}

bool    exec_pipeline(t_data *data)
{
    t_cmd   *tmp;
    int     pip[2];
    pid_t   child_pid;
    pid_t   last_pid;

    if (!data->cmds)
        return (true);
    if (data->cmds && !data->cmds->next && data->cmds->argv[0]
        && is_builtin(data->cmds->argv[0]))
        return (launch_builtin(data, data->cmds));

    last_pid = -1;
    tmp = data->cmds;
    while (tmp)
    {
        if (pipe(pip) == -1)
        {
            perror("minishell: pipe");
            return (false);
        }
        if (!exec_cmd(data, tmp, pip, &child_pid))
            return (false);
        if (tmp->next == NULL)
            last_pid = child_pid;
        tmp = tmp->next;
    }
    wait_all(data, last_pid);
    return (true);
}