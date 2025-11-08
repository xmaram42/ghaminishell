/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 14:08:52 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/06 16:13:37 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//if export and no other args
static bool	export_no_args(t_list *env)
{
	char	**arr;
	int		i;
	int		j;

	arr = lst_to_arr(env);
	if (!arr)
		return (false);
	sort_array(arr, list_length(env));
	i = 0;
	while (arr[i])
	{
		printf("declare -x ");
		j = 0;
		while (arr[i][j] && arr[i][j] != '=')
			printf("%c", arr[i][j++]);
		if (arr[i][j] && arr[i][j] == '=')
			printf("=\"%s\"\n", &arr[i][j + 1]);
		else
			printf("\n");
		i++;
	}
	free(arr);
	return (true);
}

//checks syntax
static bool	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[0] || (str[0] != '_' && !ft_isalpha(str[0])))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

//checks if identifier already in env
static int	exist(char *str, t_list *env)
{
	int		i;
	int		j;
	t_list	*tmp;

	if (!env)
		return (-1);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	j = 0;
	tmp = env;
	if (!ft_strncmp(tmp->str, str, i) && (tmp->str[i] == '\0' || \
		tmp->str[i] == '='))
		return (j);
	tmp = tmp->next;
	j++;
	while (tmp != env)
	{
		if (!ft_strncmp(tmp->str, str, i) && (tmp->str[i] == '\0' || \
			tmp->str[i] == '='))
			return (j);
		tmp = tmp->next;
		j++;
	}
	return (-1);
}

//export but norm
bool	export(char *str, t_list **env)
{
	int		pos;
	int		i;
	char	*value;

	pos = exist(str, (*env));
	value = ft_strdup(str);
	if (!value)
		return (false);
	if (pos >= 0)
	{
		i = 0;
		while (i++ < pos)
			(*env) = (*env)->next;
		free((*env)->str);
		(*env)->str = value;
	}
	else if (pos == -1)
		if (!append_to_list(env, value))
			return (false);
	return (true);
}

//export
int	ft_export(char **str, t_list **env)
{
	int	exit_code;
	int	i;

	exit_code = 0;
	i = 1;
	if (!str || !str[i])
	{
		if (*env && !export_no_args((*env)))
			return (print_error(ERR_MALLOC));
		return (0);
	}
	while (str[i])
	{
		if (!valid_identifier(str[i]))
		{
			print_error("export: not a valid identifier\n");
			exit_code = 1;
		}
		else if (!export(str[i], env))
			return (print_error(ERR_MALLOC));
		i++;
	}
	return (exit_code);
}
