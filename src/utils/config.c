/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:17:45 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/24 16:46:34 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	check_cfg(t_data *data)
{
	int	fd;

	if (access(CFG, F_OK))
	{
		fd = open(CFG, O_CREAT | O_RDWR, 0644);
		if (!fd || access(CFG, F_OK))
			cleanup(data, E_RW);
		write(fd, "COLOR=bc\n", 9);
		close (fd);
	}
}

void	read_cfg_history(t_data *data, char *read_buf)
{
	char	*history;

	history = ft_strtrim(read_buf + 8, "\n");
	add_history(history);
	if (data->last_cmd)
		free (data->last_cmd);
	data->last_cmd = ft_strdup(history);
	free (history);
}

void	read_cfg(t_data *data)
{
	char	*read_buf;

	check_cfg(data);
	if (!access(CFG, F_OK))
	{
		data->mscfg = open(CFG, O_RDWR | O_APPEND, 0644);
		if (!data->mscfg || access(CFG, F_OK))
			cleanup(data, E_RW);
		read_buf = ft_strdup("42");
		while (read_buf != NULL)
		{
			free(read_buf);
			read_buf = get_next_line(data->mscfg);
			if (read_buf && read_buf[0] == '\n')
				continue ;
			else if (read_buf && !ft_strncmp(read_buf, "COLOR", 5))
				builtin_color(data, read_buf + strlen_var(read_buf) + 1);
			else if (read_buf && !ft_strncmp(read_buf, "HISTORY", 7))
				read_cfg_history(data, read_buf);
			else
				continue ;
		}
	}
	else
		cleanup(data, E_RW);
}

// bool	builtin_dbg(t_data *data)
// {
// 	int		fd;
// 	char	buf[1];

// 	buf[0] = '\0';
// 	data->flags->debug = !data->flags->debug;
// 	if (data->flags->debug)
// 	{
// 		fd = open(CFG, O_RDWR, 0644);
// 		while (buf[0] != '\n')
// 			read(fd, buf, 1);
// 		write(fd, "DEBUG=1", 7);
// 		printf("debug mode enabled\n");
// 	}
// 	else
// 	{
// 		fd = open(CFG, O_RDWR, 0644);
// 		while (buf[0] != '\n')
// 			read(fd, buf, 1);
// 		write(fd, "DEBUG=0", 7);
// 		printf("debug mode disabled\n");
// 	}
// 	close(fd);
// 	return (true);
// }

int	strdiff(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
	i++;
	}
	if ((!s1[i] && s2[i]) || (!s2[i] && s1[i]))
		return (1);
	return (0);
}
