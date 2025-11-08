/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 16:54:07 by ghsaad            #+#    #+#             */
/*   Updated: 2025/01/15 12:34:01 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_num_of_s(char const *str, char spr)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == spr)
			i++;
		if (str[i] && str[i] != spr)
		{
			count++;
			while (str[i] && str[i] != spr)
				i++;
		}
	}
	return (count);
}

char	*ft_get_word(char const *str, int *i, int sep)
{
	char	*res;
	int		start;
	int		len;

	while (str[*i] == sep)
		(*i)++;
	start = *i;
	while (str[*i] && str[*i] != sep)
		(*i)++;
	len = *i - start;
	res = ft_substr(str, start, len);
	return (res);
}

char	**ft_split(char const *str, char c)
{
	int		i;
	int		j;
	char	**result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_num_of_s(str, c) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (j < ft_num_of_s(str, c))
	{
		result[j] = ft_get_word(str, &i, c);
		if (!result[j])
		{
			while (j--)
				free(result[j]);
			free(result);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}
