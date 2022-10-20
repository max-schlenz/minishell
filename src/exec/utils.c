/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 12:13:12 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/20 21:13:12 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_builtin(t_data *data)
{
	if (!ft_strncmp(data->argv[0], "exit", 5) \
	|| !ft_strncmp(data->argv[0], "cd", 3) \
	|| !ft_strncmp(data->argv[0], "export", 7) \
	|| !ft_strncmp(data->argv[0], "unset", 6) \
	|| !ft_strncmp(data->argv[0], "color", 6) \
	|| !ft_strncmp(data->argv[0], "history", 8) \
	|| !ft_strncmp(data->argv[0], "echo", 5) \
	|| !ft_strncmp(data->argv[0], "env", 4) \
	|| !ft_strncmp(data->argv[0], "pwd", 4) \
	|| !ft_strncmp(data->argv[0], "where", 6))
		return (true);
	return (false);
}

void	exec_close_pipes(t_data *data)
{
	int	i;

	i = 0;
	if (data->counter_pipes)
	{
		if (data->fd_i == 0)
			close (data->pipes->pipefd[0][1]);
		else if (data->fd_i < data->counter_pipes)
		{
			close (data->pipes->pipefd[data->fd_i - 1][0]);
			close (data->pipes->pipefd[data->fd_i - 1][0]);
		}
		else
		{
			i = 0;
			while (i <= data->fd_i)
			{
				close(data->pipes->pipefd[i][0]);
				close(data->pipes->pipefd[i++][1]);
			}
			data->fd_i = 0;
		}
	}
}

void	exec_set_flags(t_data *data)
{
	if (data->flags->pipe)
		data->fd_i++;
	if (data->flags->redir_out)
	{
		free_str(1, data->file_name2);
		data->file_name2 = NULL;
		data->flags->redir_out = false;
	}
	if (data->flags->redir_in)
	{
		free_str(1, data->file_name);
		data->file_name = NULL;
		data->flags->redir_in = false;
	}
	if (data->flags->redir_append)
	{
		free_str(1, data->file_name_append);
		data->file_name_append = NULL;
		data->flags->redir_append = false;
	}
	if (data->flags->heredoc)
		heredoc_set_flags(data);
	if (data->flags->pipe && (data->flags->redir_out || data->flags->redir_in))
		data->flags->pipe = false;
}

bool	check_path(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && cmd[i + 1])
	{
		if (cmd[i] && cmd[i + 1] && cmd[i] == '/' && cmd[i + 1] == '/')
			return (false);
		i++;
	}
	return (true);
}

void	builtin_fork(t_data *data, bool flag)
{
	if (!flag)
	{
		if (data->flags->pipe)
			pipe(data->pipes->pipefd[data->fd_i]);
		exec_program_create_fork(data, NULL);
	}
	else if (flag)
	{
		exec_close_pipes(data);
		exec_set_flags(data);
		data->exit_status = 0;
	}
}
