/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_p.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 22:18:57 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:13:54 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static size_t	len_hex_ptr(unsigned long num)
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

static void	print_hex_ptr_str(unsigned long num, size_t len, char *str)
{
	unsigned long		remainder;
	unsigned long		i;

	i = 0;
	while (num != 0)
	{
		remainder = num % 16;
		if (remainder < 10)
			str[len - i - 1] = 48 + remainder;
		else
			str[len - i - 1] = 55 + remainder;
		str[len - i - 1] = ft_tolower(str[len - i - 1]);
		i++;
		num /= 16;
	}
}

static int	print_hex_ptr(unsigned long ptr)
{
	unsigned long		num;
	size_t				len;
	char				*str;

	num = ptr;
	len = len_hex_ptr(num);
	str = malloc(len + 1);
	if (!str)
		return (0);
	print_hex_ptr_str(num, len, str);
	str[len] = '\0';
	ft_putstr_fd(str, 1);
	free(str);
	return (len);
}

int	print_pointer(va_list args)
{
	unsigned long	ptr;
	int				len;

	ptr = ((unsigned long)va_arg(args, void *));
	ft_putstr_fd("0x", 1);
	if (!ptr)
	{
		ft_putchar_fd('0', 1);
		return (3);
	}
	len = print_hex_ptr(ptr);
	return (len + 2);
}
