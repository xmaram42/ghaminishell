/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:10:23 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/17 14:03:37 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	g_signal_pid;

static int	make_env2(t_data *data)
{
	char	path[PATH_MAX];
	char	*tmp;

	tmp = ft_strdup("OLDPWD");
	if (!tmp || !append_to_list(&data->env, tmp))
		return (0);
	if (getcwd(path, PATH_MAX) == NULL)
		return (0);
	tmp = ft_strjoin("PWD=", path);
	if (!tmp || !append_to_list(&data->env, tmp))
		return (0);
	return (1);
}

static int	init_shell_env(t_data *data, char **envp)
{
	int		i;
	char	*tmp;

	i = 0;
	if (envp == NULL || envp[0] == NULL)
		return (make_env2(data));
	while (envp[i] != NULL)
	{
		tmp = ft_strdup(envp[i]);
		if (tmp == NULL)
		{
			free_env_list(&data->env);
			return (0);
		}
		if (!append_to_list(&data->env, tmp))
		{
			free(tmp);
			free_env_list(&data->env);
			return (0);
		}
		i = i + 1;
	}
	return (1);
}

static void	init_data(t_data *data, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	data->env = NULL;
	data->cmds = NULL;
	data->token = NULL;
	data->exit_code = 0;
	data->pip[0] = -1;
	data->pip[1] = -1;
	data->sq = false;
	data->exit_flag = false;
	g_signal_pid = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data, argc, argv);
	if (!init_shell_env(&data, envp))
	{
		ft_putstr_fd("lolipop🍭: environment initialization failed\n", 2);
		return (1);
	}
	setup_parent_signals();
	while (shell_step(&data))
		;
	shell_teardown(&data);
	return (data.exit_code);
}