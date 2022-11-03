/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 10:13:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 11:59:57 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	clears the history and removes them from the config file
static void	history_clear(t_data *data, int fd)
{
	char	tmp_cl[9];

	usleep(1000);
	read(fd, tmp_cl, 8);
	tmp_cl[8] = '\0';
	close (fd);
	fd = open(data->cfg, O_RDWR | O_TRUNC);
	if (!fd || access(data->cfg, F_OK))
		cleanup(data, E_RW);
	ft_putendl_fd(tmp_cl, 2);
	close (fd);
}

//	prints all history entries
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

//	builtin history command
//	history			print history
//	history -c		clear history
bool	builtin_history(t_data *data)
{
	int		fd;

	fd = open(data->cfg, O_RDONLY);
	if (!fd || access(data->cfg, F_OK))
		cleanup(data, E_RW);
	if (data->argv[1] && !ft_strncmp(data->argv[1], "-c", 2))
		history_clear(data, fd);
	else
		history_print(fd);
	close (fd);
	return (true);
}

//	persistent history which also compares the current command with the previous
//	one and only stores it if it's different
void	history(t_data *data)
{
	if (strdiff(data->cmd, data->last_cmd))
	{
		add_history(data->cmd);
		write(data->mscfg, "HISTORY=", 8);
		ft_putendl_fd(data->cmd, data->mscfg);
	}
	if (data->last_cmd)
		free_null (1, &data->last_cmd);
	data->last_cmd = ft_strdup(data->cmd);
}
