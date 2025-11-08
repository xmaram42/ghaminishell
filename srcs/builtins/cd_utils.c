/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 20:00:00 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/05 16:42:18 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

static char	*get_last_dir(void)
{
	static char	last_dir[PATH_MAX];

	return (last_dir);
}

void	update_last_dir(char *path)
{
	char	*last_dir;

	last_dir = get_last_dir();
	ft_strlcpy(last_dir, path, PATH_MAX);
}

int	handle_cd_prev(void)
{
	char	*last_dir;

	last_dir = get_last_dir();
	if (last_dir[0] == '\0')
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	if (handle_cd_with_path(last_dir))
		return (1);
	printf("%s\n", last_dir);
	return (0);
}

char	*create_env_var_string(char *key, char *value)
{
	char	*result;
	size_t	key_len;
	size_t	value_len;
	size_t	total_len;

	if (!key || !value)
		return (NULL);
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	total_len = key_len + value_len + 2;
	result = malloc(total_len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, key, total_len);
	ft_strlcat(result, "=", total_len);
	ft_strlcat(result, value, total_len);
	return (result);
}