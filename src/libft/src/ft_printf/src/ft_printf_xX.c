/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_xX.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 22:16:17 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:13:54 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static size_t	len_hex(unsigned int num)
{
	size_t	i;

	i = 0;
	while (num != 0)
	{
		num /= 16;
		i++;
	}
	return (i);
}

static void	print_hex_str(unsigned int num, size_t len, size_t lcase, char *str)
{
	unsigned int		i;
	unsigned int		remainder;

	i = 0;
	while (num != 0)
	{
		remainder = num % 16;
		if (remainder < 10)
			str[len - i - 1] = 48 + remainder;
		else
			str[len - i - 1] = 55 + remainder;
		if (lcase)
			str[len - i - 1] = ft_tolower(str[len - i - 1]);
		i++;
		num /= 16;
	}
}

int	print_hex(va_list args, size_t lcase)
{
	unsigned int		num;
	size_t				len;
	char				*str;

	num = va_arg(args, unsigned int);
	if (num == 0)
	{
		ft_putchar_fd('0', 1);
		return (1);
	}
	len = len_hex(num);
	str = malloc(len);
	if (!str)
		return (0);
	print_hex_str(num, len, lcase, str);
	str[len] = '\0';
	ft_putstr_fd(str, 1);
	free(str);
	return (len);
}
