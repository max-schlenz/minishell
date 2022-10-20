/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:52:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/20 17:42:48 by mschlenz         ###   ########.fr       */
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

void	redirs_pipes(t_data *data)
{
	int		fd;

	if (data->counter_pipes > 0 && data->flags->pipe)
		pipes(data);
	if (data->flags->redir_out)
	{
		fd = open(data->file_name2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_append)
	{
		fd = open(data->file_name_append, O_CREAT | O_WRONLY | O_APPEND, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_in)
	{
		fd = open(data->file_name, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	wait_for_childs(t_data *data)
{
	if (data->flags->redir_out)
	{
		open(data->file_name2, O_CREAT, 0644);
		data->flags->redir_out = false;
	}
	while (wait(&data->exit_code) > 0)
		data->exit_status = WEXITSTATUS(data->exit_code);
	data->counter_pipes = 0;
	dup2(data->fd_stdout, STDOUT_FILENO);
	close (data->fd_stdout);
}
