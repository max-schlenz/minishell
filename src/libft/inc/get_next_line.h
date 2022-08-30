/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 14:33:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/06/03 12:34:34 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
int		p_nl(const char *c);
size_t	ft_strlen(const char *c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin_dup(char const *s1, char const *s2);
char	*substr(char const *s, unsigned int start, size_t len, int flag);

#endif