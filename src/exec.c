/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 15:36:53 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 18:58:11 by tdehne           ###   ########.fr       */
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
		abs_path = ft_strjoin(abs_path, data->cmd);
		if (!access(abs_path, F_OK))
			return (abs_path);
		i++;
	}
	return (NULL);
}


bool	builtins(t_data *data)
{
	if (!ft_strncmp(data->args[0], "echo", 4))
		return (builtin_echo(data));
	else if (!ft_strncmp(data->args[0], "cd", 2))
		return (builtin_cd(data));
	else if (!ft_strncmp(data->args[0], "export", 6))
		return (builtin_export(data));
	if (!ft_strncmp(data->args[0], "exit", 4) || data->args[0] == '\0')
		cleanup(data, 0);
	return (false);
}

static void exec_cmd(t_data *data, char **envp)
{
	if (!builtins(data))
		exec_program(data, envp);
}

static void exec_cmd(t_data *data, char **envp)
{
	if (!(*data->btree)->left && !(*data->btree)->right)
		return ;
	(*data->btree) = (*data->btree)->left;
	exec_cmd(data, envp);
	if (!ft_strncmp((*data->btree)->value, "|", 2))
	{
		data->flag_pipe = true;
		return ;	
	}
	parse_args(data, (*data->btree)->value);
}

void	exec_program(t_data *data, char **envp)
{
	pid_t		pid;
	int			exit_code;
	char		*abs_path;
	
	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->cmd);
	if (!access(abs_path, F_OK))
	{
		pid = fork();
		if (pid == 0)
		{
			// dup2(data->pipes.pipefd[1], 1);
			execve(abs_path, data->args, envp);
		}
		waitpid(pid, NULL, 0);
		exit_code = WEXITSTATUS(pid);
	}
}