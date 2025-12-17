/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:54:44 by maram             #+#    #+#             */
/*   Updated: 2025/02/06 14:53:38 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return ((char *)s);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*news;
	size_t	totln;

	if (!s1)
		s1 = ft_strdup("");
	if (!s2)
		return (free(s1), NULL);
	totln = ft_strlen(s1) + ft_strlen(s2);
	news = (char *)malloc((totln + 1) * sizeof(char));
	if (!news)
		return (free(s1), NULL);
	i = 0;
	while (s1[i])
	{
		news[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		news[i++] = s2[j++];
	news[i] = '\0';
	free(s1);
	return (news);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
