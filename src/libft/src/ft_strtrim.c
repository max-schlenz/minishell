/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 18:23:01 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static size_t	is_in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(set))
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static size_t	find_first_char(char const *s1, char const *set)
{
	size_t	i;
	size_t	len_s1;

	i = 0;
	len_s1 = ft_strlen(s1);
	while (i < len_s1 && (is_in_set(s1[i], set)))
		i++;
	return (i);
}

static size_t	find_last_char(char const *s1, char const *set)
{
	size_t	i;
	size_t	len_s1;

	i = (ft_strlen(s1) - 1);
	len_s1 = ft_strlen(s1);
	while (i < len_s1 && (is_in_set(s1[i], set)))
		i--;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	str_start;
	size_t	str_end;
	size_t	str_size;
	char	*str;

	i = 0;
	if (!s1 || !set)
		return (0);
	str_start = find_first_char(s1, set);
	str_end = find_last_char(s1, set);
	if (str_start >= ft_strlen(s1))
		return (ft_strdup(""));
	str_size = (str_end - str_start + 1);
	str = malloc(str_size + 1);
	if (!str)
		return (0);
	while (i < str_size)
	{
		str[i] = s1[str_start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
