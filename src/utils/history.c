/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 10:13:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 22:52:03 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	history_clear(int fd)
{
	char	tmp_cl[9];

	usleep(1000);
	read(fd, tmp_cl, 8);
	tmp_cl[8] = '\0';
	close (fd);
	fd = open(CFG, O_RDWR | O_TRUNC);
	write(fd, tmp_cl, 8);
	write(fd, "\n", 1);
	close (fd);
}

static void	history_print(int fd)
{
	bool	flag_hist;
	bool	flag_print;
	char	tmp_print;

	flag_hist = false;
	flag_print = false;
	while (read(fd, &tmp_print, 1))
	{
		if (tmp_print == '\n' && !flag_hist)
			flag_hist = true;
		if (tmp_print == '=' && flag_hist)
		{
			read(fd, &tmp_print, 1);
			flag_print = true;
		}
		if (flag_print)
			write(1, &tmp_print, 1);
		if (tmp_print == '\n' && flag_hist)
			flag_print = false;
	}
}

bool	builtin_history(t_data *data)
{
	int		fd;

	fd = open(CFG, O_RDONLY);
	if (data->argv[1] && !ft_strncmp(data->argv[1], "-c", 2))
		history_clear(fd);
	else
		history_print(fd);
	close (fd);
	return (true);
}
