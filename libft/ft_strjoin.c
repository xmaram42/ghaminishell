/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:25:33 by ghsaad            #+#    #+#             */
/*   Updated: 2025/01/13 18:25:52 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		new_len;
	char	*new;
	int		i;
	int		j;

	new_len = ft_strlen(s1) + ft_strlen(s2);
	new = malloc((new_len + 1) * sizeof(char));
	if (new == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		new[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		new[j++] = s2[i++];
	new[j] = '\0';
	return (new);
}
