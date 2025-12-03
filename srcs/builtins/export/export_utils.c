/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 23:54:29 by maram             #+#    #+#             */
/*   Updated: 2025/12/01 00:23:44 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_var_name(char *str)
{
	int	j;

	j = 0;
	while (str[j] && str[j] != '=')
		printf("%c", str[j++]);
}

static void	print_export_var(char *str)
{
	int	j;

	printf("declare -x ");
	print_var_name(str);
	j = 0;
	while (str[j] && str[j] != '=')
		j++;
	if (str[j] && str[j] == '=')
		printf("=\"%s\"\n", &str[j + 1]);
	else
		printf("\n");
}

bool	export_no_args(t_list *env)
{
	char	**arr;
	int		i;

	arr = lst_to_arr(env);
	if (!arr)
		return (false);
	sort_array(arr, list_length(env));
	i = 0;
	while (arr[i])
	{
		print_export_var(arr[i]);
		i++;
	}
	free_array(arr);
	return (true);
}

bool	valid_identifier(char *str)
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

int	get_var_name_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}
