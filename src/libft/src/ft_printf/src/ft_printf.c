/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 12:24:36 by mschlenz          #+#    #+#             */
/*   Updated: 2022/07/02 11:26:41 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

static int	print_var(const char f_specifier, va_list args)
{
	int	ret;

	ret = 0;
	if (f_specifier == 'c')
		ret = print_char(args);
	else if (f_specifier == 's')
		ret = print_string(args);
	else if (f_specifier == 'p')
		ret = print_pointer(args);
	else if (f_specifier == 'd' || f_specifier == 'i')
		ret = print_int(args);
	else if (f_specifier == 'u')
		ret = print_uint(args);
	else if (f_specifier == 'x')
		ret = print_hex(args, 1);
	else if (f_specifier == 'X')
		ret = print_hex(args, 0);
	else if (f_specifier == '%')
	{
		ft_putchar_fd('%', 1);
		ret = 1;
	}
	return (ret);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		ret;
	int		ret_tmp;
	va_list	args;

	va_start(args, format);
	i = 0;
	ret = 0;
	ret_tmp = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			ret_tmp = print_var(format[i + 1], args);
			ret += ret_tmp - 2;
			i++;
		}
		else
			ft_putchar_fd(format[i], 1);
	i++;
	}
	va_end(args);
	return (ret + i);
}
