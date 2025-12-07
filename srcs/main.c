/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:10:23 by ghsaad            #+#    #+#             */
/*   Updated: 2025/12/07 17:06:37 by maabdulr         ###   ########.fr       */
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
	{
		free_env_list(&data->env);
		return (0);
	}
	if (getcwd(path, PATH_MAX) == NULL)
	{
		free_env_list(&data->env);
		return (0);
	}
	tmp = ft_strjoin("PWD=", path);
	if (!tmp || !append_to_list(&data->env, tmp))
	{
		free(tmp);
		free_env_list(&data->env);
		return (0);
	}
	return (1);
}

static int	add_env_entry(t_data *data, char *env_str)
{
	char	*tmp;

	tmp = ft_strdup(env_str);
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
	return (1);
}

static int	init_shell_env_after_copy(t_data *data)
{
	char	*tmp;
	char	path[PATH_MAX];

	if (!find_env_value("OLDPWD", lst_to_arr(data->env)))
	{
		tmp = ft_strdup("OLDPWD");
		if (!tmp || !append_to_list(&data->env, tmp))
			return (0);
	}
	if (!find_env_value("PWD", lst_to_arr(data->env)))
	{
		if (getcwd(path, PATH_MAX) == NULL)
			return (0);
		tmp = ft_strjoin("PWD=", path);
		if (!tmp || !append_to_list(&data->env, tmp))
			return (0);
	}
	return (1);
}

static int	init_shell_env(t_data *data, char **envp)
{
	int		i;

	i = 0;
	if (envp == NULL || envp[0] == NULL)
		return (make_env2(data));
	while (envp[i] != NULL)
	{
		if (!add_env_entry(data, envp[i]))
			return (0);
		i = i + 1;
	}
	return (init_shell_env_after_copy(data));
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data, argc, argv);
	if (!init_shell_env(&data, envp))
	{
		error_type_msg(ERR_ENV_INIT, NULL, NULL, 0);
		return (1);
	}
	setup_parent_signals();
	while (shell_step(&data))
		;
	shell_teardown(&data);
	return (data.exit_code);
}
