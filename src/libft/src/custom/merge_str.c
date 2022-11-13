/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:01:45 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/13 10:17:11 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static char	*build_str(char **strings, int len)
{	
	int		i;
	int		j;
	int		k;
	char	*ret;

	i = 0;
	j = 0;
	k = 0;
	ret = ft_calloc(len + 1, sizeof(char));
	while (strings[j])
	{
		while (strings[j][i])
			ret[k++] = strings[j][i++];
		i = 0;
		free(strings[j++]);
	}
	return (ret);
}

static char	**str_arr(va_list args, int index)
{
	int		i;
	char	**strings;
	char	*tmp;

	i = 0;
	strings = ft_calloc(index + 1, sizeof(char *));
	while (i < index)
	{
		tmp = va_arg(args, char *);
		strings[i++] = ft_strdup(tmp);
		free_null (1, &tmp);
	}
	return (strings);
}

char	*merge_str(int index, ...)
{
	va_list	args;
	char	**strings;
	char	*ret;
	int		i;
	int		len;

	va_start(args, index);
	strings = str_arr(args, index);
	va_end(args);
	i = 0;
	len = 0;
	while (strings[i])
		len += ft_strlen(strings[i++]);
	ret = build_str(strings, len);
	free (strings);
	return (ret);
}
