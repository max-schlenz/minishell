/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 13:12:40 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/25 11:01:17 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static size_t	hd_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	if (str[i] && str[i - 1] && str[i - 1] == ' ')
		i--;
	return (i);
}

static void	heredoc_prompt_output(t_data *data)
{
	char	*cmd;

	cmd = heredoc_get_cmd(data->hdoc.cmd_begin);
	if (!cmd)
		cmd = ft_strdup(data->hdoc.cmd_end + 1);
	write(2, "heredoc \x01\033[0m\x02\x01\033[38;5;239m\x02[", 29);
	write(2, cmd, hd_strlen(cmd));
	write(2, " << ", 5);
	write(2, data->hdoc.delim, ft_strlen(data->hdoc.delim) - 1);
	write(2, "] \x01\033[0;1m\x02>\x01\033[0m\x02 ", 19);
	free_str (1, cmd);
}

static int	heredoc_prompt_create_tmp(t_data *data)
{
	char	*hd_tmp_i;
	char	*hd_tmp;
	int		hd_fd;

	hd_tmp_i = ft_itoa(data->heredoc_index);
	hd_tmp = ft_strjoin(".heredoc_tmp", hd_tmp_i);
	hd_fd = open(hd_tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (!hd_fd || access(hd_tmp, F_OK))
	{
		free_str (2, hd_tmp, hd_tmp_i);
		cleanup(data, E_RW);
	}
	free_str (2, hd_tmp, hd_tmp_i);
	return (hd_fd);
}

static void	heredoc_prompt_fork(t_data *data)
{
	char	*line;
	int		hd_fd;

	heredoc_sig();
	hd_fd = heredoc_prompt_create_tmp(data);
	line = ft_strdup("42");
	while (ft_strncmp(data->hdoc.delim, line, ft_strlen(data->hdoc.delim)))
	{
		free(line);
		heredoc_prompt_output(data);
		line = get_next_line(0);
		if (!line)
			break ;
		line[ft_strlen(line)] = '\0';
		if (ft_strncmp(data->hdoc.delim, line, ft_strlen(line)))
			write(hd_fd, line, ft_strlen(line));
	}
	free_str(1, line);
	close (hd_fd);
	exit(0);
}

void	heredoc_prompt(t_data *data)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (!pid)
		heredoc_prompt_fork(data);
	waitpid(-1, &data->exit_code, 0);
	signals(false);
}
