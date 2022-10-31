/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 08:01:45 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/31 11:17:18 by mschlenz         ###   ########.fr       */
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
		free(tmp);
		tmp = NULL;
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

// char *fu(char *a, char **b)
// {
// 	*b = ft_strdup("new");
// 	return (a);
// }

// int main (void)
// {
// 	char *a = NULL;
// 	char *b = NULL;
// 	char *c = NULL;
// 	char *d = NULL;
// 	char *ret;

// 	a = ft_strdup("hello");
// 	// a = ft_strdup("test");
// 	b = ft_strdup("test");
// 	c = fu(a, &b);
// 	// free_null(1, &a);
// 	printf("%s\n", b);
// 	// d = merge_str(3, a, b, c);
// 	// free (a);
// 	// free (b);
// 	// free (c);
// 	// free (ret);
// }
