/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 10:13:07 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/04 17:17:47 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static size_t	word_count_single(char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		k = 0;
		if (s[i] == c)// && ft_strncmp(s + i, "||", 2))  // && s[i + 1] && s[i + 1] != c)
		{
			i++;
			while (s[i] == c)
			{
				k = 1;
				i++;
			}
			if (k)
				continue;
			j++;
		}
		i++;
	}
	if (s[0] != '\0')
		j++;
	// printf("%d\n", j);
	// exit(0);
	return (j);
}

static void	write_string_single(char **arr, char *buf_str, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	while (j < word_count_single(buf_str, c))
	{
		while (buf_str[k + i] && buf_str[k + i] != c)
			i++;
		if (buf_str[k + i] && buf_str[k + i + 1] && buf_str[k + i + 1] == c)
		{
			while (buf_str[k + i] == c)
				i++;
			continue ;
		}
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

char	**split_single(char *s, char c)
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
	arr = malloc(sizeof(char *) * (word_count_single(s, c) + 1));
	if (arr)
		write_string_single(arr, s, c);
	else
		return (0);
	free(buf_str);
	return (arr);
}
