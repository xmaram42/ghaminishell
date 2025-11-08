/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:09:33 by ghsaad            #+#    #+#             */
/*   Updated: 2025/01/13 19:15:55 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*arr;

	if (size && count > SIZE_MAX / size)
		return (NULL);
	arr = (void *)malloc(count * size);
	if (!(arr))
		return (NULL);
	ft_bzero(arr, (count * size));
	return (arr);
}
