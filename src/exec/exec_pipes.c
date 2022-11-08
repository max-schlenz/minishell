/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:52:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/08 19:02:35 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_pipes(t_data *data)
{
	int	i;

	if (data->fd_i == 0)
		dup2(data->pipes->pipefd[0][1], STDOUT_FILENO);
	else if (data->fd_i < data->counter_pipes)
	{
		dup2(data->pipes->pipefd[data->fd_i - 1][0], STDIN_FILENO);
		dup2(data->pipes->pipefd[data->fd_i][1], STDOUT_FILENO);
	}
	else
		dup2(data->pipes->pipefd[data->fd_i - 1][0], STDIN_FILENO);
	i = 0;
	while (i <= data->fd_i)
	{
		close(data->pipes->pipefd[i][0]);
		close(data->pipes->pipefd[i++][1]);
	}
}

int	exec_redirs_pipes_fopen(t_data *data, char **filename, int flags)
{
	int	fd;

	fd = 0;
	(void)data;
	if (!flags)
		fd = open((*filename), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (flags == 1)
		fd = open((*filename), O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (flags == 2)
		fd = open((*filename), O_RDONLY);
	if (fd == -1 || access((*filename), F_OK))
		cleanup(data, E_RW);
	free_null (1, filename);
	return (fd);
}

void	exec_redirs_pipes(t_data *data)
{
	int		fd;

	if (data->counter_pipes > 0 && data->flags->pipe)
		exec_pipes(data);
	if (data->flags->redir_out)
	{
		fd = exec_redirs_pipes_fopen(data, &data->file_name2, 0);
		data->file_name2 = NULL;
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_append)
	{
		fd = exec_redirs_pipes_fopen(data, &data->file_name_append, 1);
		data->file_name_append = NULL;
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_in)
	{
		fd = exec_redirs_pipes_fopen(data, &data->file_name, 2);
		data->file_name = NULL;
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void	exec_wait_for_childs(t_data *data)
{
	data->exit_code = 0;
	if (data->flags->redir_out && data->file_name2)
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
	close_pipes(data);
}
