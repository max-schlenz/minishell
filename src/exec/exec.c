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

static bool	exec_err(t_data *data, DIR *tmp, char *abs_path)
{
	if (tmp)
		data->exit_status = 126;
	else
		data->exit_status = 127;
	perror("Error ");
	free (abs_path);
	if (tmp)
		closedir(tmp);
	return (true);
}

void	exec_program_create_fork(t_data *data)
{
	if (data->flags->pipe)
		pipe(data->pipes->pipefd[data->fd_i]);
	signal(SIGINT, SIG_IGN);
	data->pid = fork();
	if (data->pid == -1)
		ms_exit(2, WEXITSTATUS(data->exit_code));
}

static void	exec_program_child(t_data *data, char *abs_path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redirs_pipes(data);
	if (!builtin_print(data))
	{
		if (!access(abs_path, F_OK))
			execve(abs_path, data->argv, data->envp);
	}
	exit(0);
}

bool	exec_program(t_data *data)
{
	char		*abs_path;
	DIR			*tmp;

	data->pid = 1;
	data->flags->error = false;
	abs_path = get_path(data, data->argv[0]);
	if (!abs_path && data->argv[0])
		abs_path = ft_strdup(data->argv[0]);
	tmp = opendir(abs_path);
	if (!tmp && (!access(abs_path, F_OK) || is_builtin(data)))
		exec_program_create_fork(data);
	else
		data->flags->error = true;
	if (data->pid == 0)
		exec_program_child(data, abs_path);
	if (data->flags->error)
		return (exec_err(data, tmp, abs_path));
	free (abs_path);
	if (tmp)
		closedir(tmp);
	exec_close_pipes(data);
	exec_set_flags(data);
	return (true);
}
