/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:06:08 by ghsaad            #+#    #+#             */
/*   Updated: 2025/01/13 18:38:54 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*dst;
	char	*source;
	size_t	i;

	source = (char *)src;
	dst = (char *)dest;
	i = 0;
	if (!source && !dest)
		return (0);
	while (i < n)
	{
		dst[i] = source[i];
		i++;
	}
	return (dst);
}
