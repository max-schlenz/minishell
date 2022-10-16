/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 12:13:12 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/15 18:28:59 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_builtin(t_data *data)
{
	if ((!ft_strncmp(data->argv[0], "exit", 5))
		|| (!ft_strncmp(data->argv[0], "cd", 3))
		|| (!ft_strncmp(data->argv[0], "export", 7))
		|| (!ft_strncmp(data->argv[0], "unset", 6))
		|| (!ft_strncmp(data->argv[0], "color", 6))
		|| (!ft_strncmp(data->argv[0], "history", 8))
		|| (!ft_strncmp(data->argv[0], "echo", 5))
		|| (!ft_strncmp(data->argv[0], "env", 4))
		|| (!ft_strncmp(data->argv[0], "pwd", 4)))
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
		}
	}
}

void	exec_set_flags(t_data *data)
{
	data->fd_i++;
	if (data->flags->redir_out)
		data->flags->redir_out = false;
	if (data->flags->redir_in)
	{
		free(data->file_name);
		data->file_name = NULL;
		data->flags->redir_in = false;
	}
	if (data->flags->redir_append)
	{
		free(data->file_name_append);
		data->file_name_append = NULL;
		data->flags->redir_append = false;
	}
	if (data->flags->heredoc)
	{
		data->heredoc_index++;
		data->flags->heredoc = false;
	}
	if (data->flags->pipe && (data->flags->redir_out || data->flags->redir_in))
		data->flags->pipe = false;
}

char	*get_path(t_data *data, char *cmd)
{
	int		i;
	char	*abs_path_bs;
	char	*abs_path;
	char	*cmd_trim;

	i = 0;
	while (data->path && data->path[i])
	{
		if (ft_strchr(cmd + 2, '/'))
			return (NULL);
		abs_path_bs = ft_strjoin(data->path[i], "/");
		cmd_trim = ft_strtrim(cmd, " ");
		abs_path = merge_str(2, abs_path_bs, cmd_trim);
		if (!access(abs_path, F_OK))
			return (abs_path);
		free (abs_path);
		i++;
	}
	return (NULL);
}

void	builtin_fork(t_data *data, bool flag)
{
	if (!flag)
		exec_program_create_fork(data);
	else if (flag)
	{
		exec_close_pipes(data);
		exec_set_flags(data);
		data->exit_status = 0;
	}
}