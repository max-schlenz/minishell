/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 16:23:40 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	char	*buf_haystack;

	i = 0;
	buf_haystack = ((char *)haystack);
	if ((ft_strlen(needle)) == 0 || haystack == needle)
		return (buf_haystack);
	while (i < len && buf_haystack[i] != '\0')
	{
		j = 0;
		while (i + j < len && buf_haystack[i + j] != '\0' && needle[j] != '\0'
			&& buf_haystack[i + j] == needle[j])
			j++;
		if (j == (ft_strlen(needle)))
			return (buf_haystack + i);
		i++;
	}
	return (0);
}
