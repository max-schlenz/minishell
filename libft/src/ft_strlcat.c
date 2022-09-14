/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 14:57:24 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;

	dst_len = ft_strlen(dst);
	i = dst_len;
	j = 0;
	if (i >= dstsize || dstsize == 0)
		return (dstsize + ft_strlen(src));
	while (src[j] != '\0' && i < dstsize - 1)
	{
	dst[i] = ((char *)src)[j];
	i++;
	j++;
	}
	dst[i] = '\0';
	return (dst_len + ft_strlen(src));
}
