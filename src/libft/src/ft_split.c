/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 10:13:07 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static size_t	word_count(char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			j++;
		i++;
	}
	if (s[0] != '\0')
		j++;
	return (j);
}

static void	write_string(char **arr, char *buf_str, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	while (j < word_count(buf_str, c))
	{
		while (buf_str[k + i] != '\0' && buf_str[k + i] != c)
			i++;
		arr[j] = malloc(sizeof(char) * (i + 1));
		if (arr[j])
			ft_strlcpy(arr[j], buf_str + k, i + 1);
		k += (ft_strlen(arr[j]));
		while (buf_str[k] != '\0' && buf_str[k] == c)
			k++;
		j++;
		i = 0;
	}
	arr[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	char	*buf_str;
	char	buf_c[2];

	if (!s)
		return (NULL);
	buf_c[0] = c;
	buf_c[1] = '\0';
	buf_str = ft_strtrim(s, buf_c);
	if (!buf_str)
		return (NULL);
	arr = malloc(sizeof(char *) * (word_count(buf_str, c) + 1));
	if (arr)
		write_string(arr, buf_str, c);
	else
		return (0);
	free(buf_str);
	return (arr);
}
