/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 21:00:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/07/29 14:43:00 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>
#include <libft.h>

static void	create_line(char **line, char **stat_buf, int fd, int rcheck)
{
	char			*buf;
	char			*ptr_old_line;
	char			*buf_substr_nl;

	ptr_old_line = NULL;
	buf_substr_nl = NULL;
	buf = ft_calloc(BUFFER_SIZE + 1, 1);
	while (rcheck > 0)
	{
		rcheck = read(fd, buf, BUFFER_SIZE);
		buf[rcheck] = '\0';
		ptr_old_line = *line;
		if (p_nl(buf) >= 0)
		{
			*stat_buf = substr(buf, p_nl(buf) + 1, BUFFER_SIZE - p_nl(buf), 1);
			buf_substr_nl = substr(buf, 0, p_nl(buf), 2);
			*line = ft_strjoin_dup(*line, buf_substr_nl);
			free (buf_substr_nl);
			free (ptr_old_line);
			break ;
		}
		*line = ft_strjoin_dup(*line, buf);
		free(ptr_old_line);
	}
	free (buf);
}

static void	trim_stat_buf(char **stat_buf)
{
	int		stat_buf_nl;
	char	*ptr_stat_buf;

	stat_buf_nl = p_nl(*stat_buf);
	ptr_stat_buf = *stat_buf;
	*stat_buf = substr(*stat_buf, stat_buf_nl + 1, \
				ft_strlen(*stat_buf) - stat_buf_nl, 1);
	free (ptr_stat_buf);
}

char	*get_next_line(int fd)
{
	static char		*stat_buf = NULL;
	char			*line;

	line = NULL;
	if (BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	if (stat_buf)
	{
		if (p_nl(stat_buf) == -1)
			line = ft_strdup(stat_buf);
		else
		{
			line = substr(stat_buf, 0, p_nl(stat_buf), 2);
			trim_stat_buf(&stat_buf);
			return (line);
		}
		free (stat_buf);
		stat_buf = NULL;
	}
	create_line(&line, &stat_buf, fd, 1);
	if (ft_strlen(line) > 0)
		return (line);
	free (line);
	return (NULL);
}
