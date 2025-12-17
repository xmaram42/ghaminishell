/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:10:31 by maabdulr          #+#    #+#             */
/*   Updated: 2025/02/06 18:35:13 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*read_and_store(int fd, char *rem)
{
	char	*buffer;
	char	*temp;
	int		readed;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	readed = 1;
	while (readed > 0 && !ft_strchr(rem, '\n'))
	{
		readed = read(fd, buffer, BUFFER_SIZE);
		if (readed < 0)
		{
			free(buffer);
			free(rem);
			return (NULL);
		}
		buffer[readed] = '\0';
		temp = ft_strjoin(rem, buffer);
		if (!temp)
			return (free(buffer), NULL);
		rem = temp;
	}
	free(buffer);
	return (rem);
}

char	*extract_line(char *rem)
{
	char	*new;
	char	*line;
	int		len;
	int		i;

	if (!rem)
		return (NULL);
	new = ft_strchr(rem, '\n');
	if (!new)
		return (ft_strdup(rem));
	len = (new - rem) + 1;
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = rem[i];
		i++;
	}
	line[len] = '\0';
	return (line);
}

char	*update_rem(char *rem)
{
	char	*new;
	char	*new_rem;

	if (!rem)
		return (NULL);
	new = ft_strchr(rem, '\n');
	if (!new)
	{
		free(rem);
		return (NULL);
	}
	if (*(new + 1) == '\0')
	{
		free(rem);
		return (NULL);
	}
	new_rem = ft_strdup(new + 1);
	if (!new_rem)
	{
		free(rem);
		return (NULL);
	}
	free(rem);
	return (new_rem);
}

char	*get_next_line(int fd)
{
	static char	*rem[OPEN_MAX];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	rem[fd] = read_and_store(fd, rem[fd]);
	if (!rem[fd] || *rem[fd] == '\0')
	{
		free(rem[fd]);
		rem[fd] = NULL;
		return (NULL);
	}
	line = extract_line(rem[fd]);
	rem[fd] = update_rem(rem[fd]);
	return (line);
}
