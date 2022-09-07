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
	if (!ft_strncmp(data->argv[0], "echo", 5))
		return (builtin_echo(data));
	if (!ft_strncmp(data->argv[0], "cd", 3))
		return (builtin_cd(data));
	else if (!ft_strncmp(data->argv[0], "export", 7))
		return (builtin_export(data));
	else if (!ft_strncmp(data->argv[0], "env", 4))
		return (builtin_env(data));
	if (!ft_strncmp(data->argv[0], "exit", 5)) // || data->argv[0] == '\0')
		cleanup(data, 0);
	return (false);
}

void	exec_program(t_data *data)
{
	pid_t		pid;
	int			exit_code;
	char		*abs_path;
	static int i = 0;

	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->cmd);
	if (!access(abs_path, F_OK))
	{
		pid = fork();
		if (pid == 0)
		{
			if (i == 0)
				dup2(data->pipefd[0][1], 1);				//stdout <-> fd 0 write
			else if (i == 1)
			{
				dup2(data->pipefd[0][0], 0);				//stdin <-> fd 0 read
				dup2(data->pipefd[1][1], 1);				//stdout <-> fd 1 write
			}
			else if (i == 2)
			{	
				dup2(data->pipefd[1][0], 0);				//stdin <-> fd 1 read
				dup2(data->pipefd[2][1], 1);				//stdout <-> fd 2 write
			}
			else if (i == 3)
			{	
				dup2(data->pipefd[2][0], 0);				//stdin <-> fd 2 read
				dup2(data->pipefd[3][1], 1);				//stdout <-> fd 3 write
			}
			else if (i == 4)
				dup2(data->pipefd[3][0], 0);				//stdin <-> fd 3 read
			execve(abs_path, data->argv, data->envp);
		}
		waitpid(pid, &exit_code, 0);
		if (i != 4)
		close(data->pipefd[i][1]);						//still the only close that matters lol
		data->flag_pipe = !data->flag_pipe;
		if (i == data->counter_pipes)
		{
			i = 0;
			return ;
		}
		i++;
		return ;
	}
	printf("command %s not found\n", data->argv[0]);
}
