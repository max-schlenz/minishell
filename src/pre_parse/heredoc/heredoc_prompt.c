/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 13:12:40 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/11 14:50:31 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	heredoc_prompt_output(t_data *data)
{
	write(2, "heredoc \x01\033[0m\x02\x01\033[38;5;239m\x02[", 29);
	write(2, data->hdoc.delim, ft_strlen(data->hdoc.delim) - 1);
	write(2, "] \x01\033[0;1m\x02>\x01\033[0m\x02 ", 19);
}

static int	heredoc_prompt_create_tmp(t_data *data)
{
	char	*hd_tmp_i;
	char	*hd_tmp;
	char	*home_tmp;
	int		hd_fd;

	hd_tmp_i = ft_itoa(data->heredoc_index);
	hd_tmp = ft_strjoin(".heredoc_tmp", hd_tmp_i);
	hd_fd = open(hd_tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (!hd_fd || access(hd_tmp, F_OK))
	{
		home_tmp = expand_get_var_content(data, "~");
		hd_tmp = realloc_ptr(hd_tmp, ft_strjoin(home_tmp, "/"), true);
		hd_tmp = realloc_ptr(hd_tmp, ft_strjoin(hd_tmp, ".heredoc_tmp"), true);
		hd_tmp = realloc_ptr(hd_tmp, ft_strjoin(hd_tmp, hd_tmp_i), true);
		free_null (1, &home_tmp);
		hd_fd = open(hd_tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (!hd_fd || access(hd_tmp, F_OK))
		{
			free_null (2, &hd_tmp, &hd_tmp_i);
			cleanup(data, E_RW);
		}
	}
	free_null (2, &hd_tmp, &hd_tmp_i);
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
		line = heredoc_vars(data, line);
		line[ft_strlen(line)] = '\0';
		if (ft_strncmp(data->hdoc.delim, line, ft_strlen(line)))
			write(hd_fd, line, ft_strlen(line));
	}
	free_null(1, &line);
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
	if (pid == -1)
		ms_exit(E_FORK, WEXITSTATUS(data->exit_code));
	waitpid(-1, &data->exit_code, 0);
	signals(false);
}
