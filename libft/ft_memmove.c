/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:02:34 by ghsaad            #+#    #+#             */
/*   Updated: 2025/01/13 19:07:13 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	char	*srcc;
	char	*dstt;

	srcc = (char *)src;
	dstt = (char *)dst;
	if (dst == src)
		return (dst);
	i = -1;
	if (src > dst)
	{
		while (len > ++i)
			dstt[i] = srcc[i];
	}
	else
	{
		while (len)
		{
			dstt[len - 1] = srcc[len - 1];
			len--;
		}
	}
	return (dst);
}
