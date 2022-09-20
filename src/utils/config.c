/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:17:45 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/20 13:49:03 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	create_config()
{
	int fd;

	fd = open(".mscfg", O_CREAT | O_RDWR, 0644);
	write(fd, "COLOR=bc\n", 9);
	close (fd);
}

void	read_config(t_data *data)
{
	char *read_buf;
	char *history;

	if (access(".mscfg", F_OK))
		create_config();
	if (!access(".mscfg", F_OK))
	{
		data->mscfg = open(".mscfg", O_RDWR | O_APPEND, 0644);
		read_buf = ft_strdup("42");
		while (read_buf != NULL)
		{
			free(read_buf);
			read_buf = get_next_line(data->mscfg);
			if (read_buf && read_buf[0] == '\n')
				continue ;
			if (read_buf && !strcmp_alnum(read_buf, "COLOR"))
				builtin_color(data, read_buf + strlen_var(read_buf) + 1);
			if (read_buf && !strcmp_alnum(read_buf, "HISTORY"))
			{
				history = ft_strtrim(read_buf + 8, "\n");
				add_history(history);
				if (data->last_cmd)
					free (data->last_cmd);
				data->last_cmd = ft_strdup(history);
				free (history);
			}
		}
		return ;
	}
}