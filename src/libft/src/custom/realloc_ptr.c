/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:15:01 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 09:46:49 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*realloc_ptr(char *ptr, char *new, bool free_new)
{
	char	*ret;

	free_null (1, &ptr);
	ret = ft_strdup(new);
	if (free_new)
		free_null (1, &new);
	return (ret);
}
