/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 20:35:24 by mschlenz          #+#    #+#             */
/*   Updated: 2022/07/29 14:42:52 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>

int	p_nl(const char *c)
{
	int	i;

	i = 0;
	while (c[i] != '\0')
	{
		if (c[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_strjoin_dup(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (ft_strdup(s2));
	str = malloc(ft_strlen(s1) + (ft_strlen(s2) + 1));
	if (!str)
		return (0);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

char	*substr(char const *s, unsigned int start, size_t len, int flag)
{
	size_t	i;
	char	*str;

	i = 0;
	if (start >= (ft_strlen(s)))
	len = 0;
	if (len > (ft_strlen(s)))
		len = (ft_strlen(s));
	str = malloc(len + flag);
	if (!str)
		return (0);
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	if (flag == 1)
		str[i] = '\0';
	else
	{
		str[i] = '\n';
		str[i + 1] = '\0';
	}
	return (str);
}
