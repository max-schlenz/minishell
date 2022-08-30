/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_csi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 22:20:47 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:25:12 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>

int	print_char(va_list args)
{
	char	c;

	c = va_arg(args, int);
	ft_putchar_fd(c, 1);
	return (1);
}

int	print_string(va_list args)
{
	char	*str;

	str = va_arg(args, char *);
	if (str == NULL)
		str = "(null)";
	ft_putstr_fd(str, 1);
	return (ft_strlen(str));
}

int	print_int(va_list args)
{
	char	*str;
	size_t	len;

	str = ft_itoa(va_arg(args, int));
	ft_putstr_fd(str, 1);
	len = ft_strlen(str);
	free(str);
	return (len);
}
