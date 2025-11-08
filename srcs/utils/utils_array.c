/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:35:59 by ghsaad            #+#    #+#             */
/*   Updated: 2025/10/30 18:21:16 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**lst_to_arr(t_list *env)
{
	t_list	*lst;
	char	**dest;
	int		i;

	if (!env)
		return (NULL);
	dest = malloc(sizeof(char *) * (list_length(env) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	lst = env;
	dest[i] = ft_strdup(lst->str);  // CHANGED: Use ft_strdup instead of direct pointer
	lst = lst->next;
	i++;
	while (lst != env)
	{
		dest[i] = ft_strdup(lst->str);  // CHANGED: Use ft_strdup
		lst = lst->next;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}


//swap to elems in array
static void	ft_swap_str_tab(int i, int j, char **tab)
{
	char	*temp;

	temp = tab[i];
	tab[i] = tab[j];
	tab[j] = temp;
}

//sorts array
void	sort_array(char **arr, int len)
{
	int	i;
	int	j;
	int	diff;

	i = 0;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			diff = ft_strncmp(arr[i], arr[j], __INT_MAX__);
			if (diff > 0)
			{
				ft_swap_str_tab(i, j, arr);
				continue ;
			}
			j++;
		}
	i++;
	}
}
