/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 17:42:43 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static int	calc_req_chars(int n)
{
	int	r;

	r = 1;
	while (n > 9 || n < -9)
	{
		n = n / 10;
		r++;
	}
	return (r);
}

static void	create_string(char *str, int n, int i)
{
	if (n)
	{
		while (n != 0)
		{
			if (n > 0)
				str[i - 1] = n % 10 + '0';
			else
				str[i - 1] = n % 10 * (-1) + '0';
			n /= 10;
			i--;
		}
	}
	else
		str[0] = '0';
}

char	*ft_itoa(int n)
{
	char	*str;
	int		digit_count;
	int		i;
	int		is_neg;

	is_neg = 0;
	if (n < 0)
	{
			n *= -1;
			is_neg = 1;
	}
	digit_count = calc_req_chars(n);
	i = digit_count + is_neg;
	str = malloc(digit_count + is_neg + 1);
	if (!str)
		return (0);
	create_string(str, n, i);
	if (is_neg)
		str[0] = '-';
	str[digit_count + is_neg] = '\0';
	return (str);
}
