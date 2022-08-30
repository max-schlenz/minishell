/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 14:33:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:09:53 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <libft.h>

int		ft_printf(const char *format, ...);
int		print_char(va_list args);
int		print_string(va_list args);
int		print_pointer(va_list args);
int		print_int(va_list args);
int		print_uint(va_list args);
int		print_hex(va_list args, size_t lcase);
size_t	ft_strlen(const char *c);
int		ft_tolower(int c);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_itoa(int n);

#endif