/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 15:36:53 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/05 13:43:24 by mschlenz         ###   ########.fr       */
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
	else if (!ft_strncmp(data->argv[0], "cd", 2))
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
	char		*blub[3];

	blub[0] = "/usr/bin/grep";
	blub[1] = "l";
	blub[2] = NULL;
	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->cmd);
	pid = fork();
	if (pid == 0)
	{
		execve(abs_path, data->argv, data->envp);
		// if (!data->flag_pipe)
		// {
		// 	dup2(data->pipes->pipefd[1], 1);
		// 	close(data->pipes->pipefd[0]);
		// 	data->flag_pipe = true;
		// 	execve(abs_path, data->args, data->envp);
		// }
		// else
		// {
		//  	dup2(data->pipes->pipefd[0], 0);
		//  	close(data->pipes->pipefd[1]);
		// }
	}
	waitpid(pid, &exit_code, 0);
	// exit_code = WEXITSTATUS(pid);
}
