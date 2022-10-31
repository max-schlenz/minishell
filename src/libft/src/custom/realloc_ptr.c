/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:15:01 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/31 11:17:06 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*realloc_ptr(char *ptr, char *new, bool free_new)
{
	char	*ret;

	free (ptr);
	ret = ft_strdup(new);
	if (free_new)
		free_null (1, &new);
	return (ret);
}
