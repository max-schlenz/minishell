/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 15:36:53 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/06 15:32:31 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char *get_path(t_data *data)
{
	int		i;
	char	*abs_path_bs;
	char	*abs_path;
	char	*cmd_trim;

	i = 0;
	while (data->path[i])
	{
		abs_path_bs = ft_strjoin(data->path[i], "/");
		cmd_trim = ft_strtrim(data->argv[0], " ");
		abs_path = ft_strjoin(abs_path_bs, cmd_trim);
		free (abs_path_bs);
		free (cmd_trim);
		if (!access(abs_path, F_OK))
			return (abs_path);
		free (abs_path);
		i++;
	}
	return (NULL);
}

bool	builtin_print(t_data *data)
{
	if (!ft_strncmp(data->argv[0], "echo", 5))
		return (builtin_echo(data));
	else if (!ft_strncmp(data->argv[0], "env", 4))
		return (builtin_env(data));
	else if (!ft_strncmp(data->argv[0], "pwd", 4))
		return (builtin_pwd(data));
	return (false);
}

bool	builtin_environment(t_data *data)
{
	if (!ft_strncmp(data->argv[0], "exit", 5)) // || data->argv[0] == '\0')
			cleanup(data, 0);
	else if (!ft_strncmp(data->argv[0], "cd", 3))
		return (builtin_cd(data));
	else if (!ft_strncmp(data->argv[0], "export", 7))
		return (builtin_export(data));
	else if (!ft_strncmp(data->argv[0], "unset", 6))
		return (builtin_unset(data));
	else if (!ft_strncmp(data->argv[0], "color", 6))
		return (builtin_color(data, NULL));
	else if (!ft_strncmp(data->argv[0], "history", 8))
		return (builtin_history(data));
	return (false);
}

void	pipes(t_data *data)
{
	if (data->fd_i == 0)
		dup2(data->pipes->pipefd[0][1], 1);								//stdout <-> fd 0 write
	else if (data->fd_i < data->counter_pipes)
	{
		dup2(data->pipes->pipefd[data->fd_i - 1][0], 0);				//stdin <-> fd 0 read
		dup2(data->pipes->pipefd[data->fd_i][1], 1);			 		//stdout <-> fd 1 write
	}
	else
		dup2(data->pipes->pipefd[data->fd_i - 1][0], 0);				//stdin <-> fd 3 read
}

bool	exec_program(t_data *data)
{
	pid_t		pid;
	int			exit_code;
	char		*abs_path;
	int			fd;
	
	int			redir_delim_fd[2];
	char		*redir_delim_tmp;
	char		rdbuf[4096];

	exit_code = 0;
	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->argv[0]);
	
	pid = fork();
	if (pid == -1)
		ft_exit(2);
	if (pid == 0)
	{
		if (data->flags->redir_out)
		{
			fd = open(data->file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		if (data->flags->redir_in_delim)
		{
			pipe(redir_delim_fd);
			redir_delim_tmp = ft_strdup("42");
			while (ft_strncmp(redir_delim_tmp, data->heredoc_delim, ft_strlen(data->heredoc_delim)))
			{
				free(redir_delim_tmp);
				redir_delim_tmp = get_next_line(0);
				redir_delim_tmp[ft_strlen(redir_delim_tmp)] = '\0';
				if (ft_strncmp(data->heredoc_delim, redir_delim_tmp, ft_strlen(redir_delim_tmp)))
					write(redir_delim_fd[1], redir_delim_tmp, ft_strlen(redir_delim_tmp));
			}
			free(redir_delim_tmp);
			close(redir_delim_fd[1]);
			dup2(redir_delim_fd[0], STDIN_FILENO);
			close(redir_delim_fd[0]);
		}
		else if (data->flags->redir_out_append)
		{
			fd = open(data->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (data->flags->redir_in)
		{
			fd = open(data->file_name, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (data->counter_pipes > 0 && data->flags->pipe)
			pipes(data);
		if (!builtin_print(data))
		{
			if (!access(abs_path, F_OK))
				execve(abs_path, data->argv, data->envp);
		}
		exit(0);
	}
	waitpid(pid, &exit_code, 0);
	if (data->counter_pipes > 0 && data->fd_i != data->counter_pipes)
		close(data->pipes->pipefd[data->fd_i][1]);
	// if (!data->flags->redir_out)
		data->fd_i++;
	free (abs_path);
	if (data->flags->pipe && (data->flags->redir_out || data->flags->redir_in))
		data->flags->pipe = false;
	if (data->flags->redir_out)
		data->flags->redir_out = false;
	if (data->flags->redir_in)
		data->flags->redir_in = false;
	if (data->flags->redir_out_append)
		data->flags->redir_out_append = false;
	if (data->flags->redir_in_delim)
		data->flags->redir_in_delim = false;
	data->exit_status = WEXITSTATUS(exit_code);
	return (true);
}

