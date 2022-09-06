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
	char	*abs_path;

	i = 0;
	while (data->path[i])
	{
		abs_path = ft_strjoin(data->path[i], "/");
		abs_path = ft_strjoin(abs_path, data->argv[0]);
		if (!access(abs_path, F_OK))
			return (abs_path);
		i++;
	}
	return (NULL);
}


bool	builtins(t_data *data)
{
	if (!ft_strncmp(data->argv[0], "echo", 4))
		return (builtin_echo(data));
	if (!ft_strncmp(data->argv[0], "cd", 2))
		return (builtin_cd(data));
	else if (!ft_strncmp(data->argv[0], "export", 6))
		return (builtin_export(data));
	if (!ft_strncmp(data->argv[0], "exit", 4)) // || data->argv[0] == '\0')
		cleanup(data, 0);
	return (false);
}

void	exec_program(t_data *data)
{
	pid_t		pid;
	int			exit_code;
	char		*abs_path;

	static int kk = 0;
	kk++;
	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->cmd);
	pid = fork();
	if (pid == 0)
	{
		if (kk == 3)
		{
			dup2(data->pipes->pipefd[1][0], 0);
			execve(abs_path, data->argv, data->envp);
		}
		else if (data->flag_pipe == 0)
		{
			dup2(data->pipes->pipefd[0][1], 1);
			close(data->pipes->pipefd[0][0]);
			execve(abs_path, data->argv, data->envp);
		}
		else if (data->flag_pipe == 1)
		{
			dup2(data->pipes->pipefd[0][0], 0);
			dup2(data->pipes->pipefd[1][1], 1);
			execve(abs_path, data->argv, data->envp);
		}
	}
	waitpid(pid, &exit_code, 0);
	close(data->pipes->pipefd[data->flag_pipe][1]);
	data->flag_pipe = !data->flag_pipe;
}
