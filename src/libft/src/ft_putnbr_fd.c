/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 20:00:37 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	buf_n;

	buf_n = n;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		buf_n *= -1;
	}
	if (buf_n > 9)
	{
		ft_putnbr_fd((buf_n / 10), fd);
		ft_putnbr_fd((buf_n % 10), fd);
	}
	else
		ft_putchar_fd((buf_n + '0'), fd);
}
