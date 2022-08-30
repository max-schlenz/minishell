/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 22:19:12 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:13:54 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static size_t	uitoa_calc_req_chars(unsigned int n)
{
	int	r;

	r = 1;
	while (n > 9)
	{
		n = n / 10;
		r++;
	}
	return (r);
}

static void	uitoa_create_string(char *str, unsigned n, unsigned i)
{
	if (n)
	{
		while (n != 0)
		{
			str[i - 1] = n % 10 + '0';
			n /= 10;
			i--;
		}
	}
	else
		str[0] = '0';
}

static char	*uitoa(unsigned int n)
{
	char				*str_uint;
	unsigned int		digit_count;
	unsigned int		i;

	digit_count = uitoa_calc_req_chars(n);
	i = digit_count;
	str_uint = malloc(digit_count + 1);
	if (!str_uint)
		return (0);
	uitoa_create_string(str_uint, n, i);
	str_uint[digit_count] = '\0';
	return (str_uint);
}

int	print_uint(va_list args)
{
	char		*str;
	size_t		len;

	str = uitoa(va_arg(args, unsigned int));
	ft_putstr_fd(str, 1);
	len = ft_strlen(str);
	free(str);
	return (len);
}
