/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:52:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/24 16:56:56 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pipes(t_data *data)
{
	int	i;

	if (data->fd_i == 0)
		dup2(data->pipes->pipefd[0][1], 1);
	else if (data->fd_i < data->counter_pipes)
	{
		dup2(data->pipes->pipefd[data->fd_i - 1][0], 0);
		dup2(data->pipes->pipefd[data->fd_i][1], 1);
	}
	else
		dup2(data->pipes->pipefd[data->fd_i - 1][0], 0);
	i = 0;
	while (i <= data->fd_i)
	{
		close(data->pipes->pipefd[i][0]);
		close(data->pipes->pipefd[i++][1]);
	}
}

int	redirs_pipes_fopen(t_data *data, char *filename, int flags)
{
	int	fd;

	fd = 0;
	(void)data;
	if (!flags)
		fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (flags == 1)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (flags == 2)
		fd = open(filename, O_RDONLY);
	if (fd == -1 || access(filename, F_OK))
		cleanup(data, E_RW);
	return (fd);
}

void	redirs_pipes(t_data *data)
{
	int		fd;

	if (data->counter_pipes > 0 && data->flags->pipe)
		pipes(data);
	if (data->flags->redir_out)
	{
		fd = redirs_pipes_fopen(data, data->file_name2, 0);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_append)
	{
		fd = redirs_pipes_fopen(data, data->file_name_append, 1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_in)
	{
		fd = redirs_pipes_fopen(data, data->file_name, 2);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	wait_for_childs(t_data *data)
{
	data->exit_code = 0;
	if (data->flags->redir_out)
	{
		if (!open(data->file_name2, O_CREAT, 0644))
			cleanup(data, E_RW);
		data->flags->redir_out = false;
	}
	data->fd_i = 0;
	while (waitpid(-1, &data->exit_code, 0) != -1)
	{
		if (WIFEXITED(data->exit_code))
			data->exit_status = WEXITSTATUS(data->exit_code);
	}
	data->counter_pipes = 0;
	data->flags->pipe = false;
	dup2(data->fd_stdout, STDOUT_FILENO);
	close (data->fd_stdout);
	// dup2(data->fd_stdin, STDIN_FILENO);
	// close (data->fd_stdin);
	close_pipes(data);
}
