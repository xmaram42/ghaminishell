/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghsaad <ghsaad@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:42:22 by ghsaad            #+#    #+#             */
/*   Updated: 2025/10/21 14:24:45 by ghsaad           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_array(char **array)
{
    int	i;

    if (array == NULL)
        return ;
    i = 0;
    while (array[i] != NULL)
    {
        free(array[i]);
        i++;
    }
    free(array);
    array = NULL;
}
/*
this function frees every string in a NULL-terminated array of strings,
then frees the array itself and sets the pointer to NULL
*/
