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

char	*exec_get_path(t_data *data, char *cmd)
{
	int		i;
	char	*abs_path_tmp;
	char	*abs_path;
	char	*cmd_trim;

	i = 0;
	if (!access(cmd, F_OK) && exec_check_path(cmd))
		return (ft_strdup(cmd));
	while (data->path && data->path[i])
	{
		if (ft_strlen(cmd) > 2 && ft_strchr(cmd + 2, '/'))
			break ;
		abs_path_tmp = ft_strjoin(data->path[i], "/");
		cmd_trim = ft_strtrim(cmd, " ");
		abs_path = merge_str(2, abs_path_tmp, cmd_trim);
		if (!access(abs_path, F_OK))
			return (abs_path);
		free_null (1, &abs_path);
		i++;
	}
	return (NULL);
}

static bool	exec_error_fork(t_data *data, char *abs_path, bool isdir)
{
	char	*msg;

	if (isdir)
	{
		msg = E_EXEC_ISDIR;
		data->exit_status = 126;
	}
	else
	{
		msg = E_EXEC_NOTFOUND;
		data->exit_status = 127;
		abs_path = ft_strdup(data->argv[0]);
	}
	if (data->flags->pipe)
	{
		exec_close_pipes(data);
		exec_set_flags(data);
		data->flags->pipe = false;
	}
	write(2, "Error: ", 8);
	ft_putstr_fd(abs_path, 2);
	ft_putendl_fd(msg, 2);
	free_null (1, &abs_path);
	return (true);
}

static void	exec_child(t_data *data, char *abs_path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	exec_redirs_pipes(data);
	if (abs_path && !access(abs_path, F_OK))
	{
		if (execve(abs_path, data->argv, data->envp) == -1)
			exec_error_fork(data, abs_path, false);
	}
	exit(data->exit_status);
}

void	exec_create_fork(t_data *data, char *abs_path)
{
	signal(SIGINT, SIG_IGN);
	if (data->flags->pipe)
		pipe(data->pipes->pipefd[data->fd_i]);
	data->pid = fork();
	if (data->pid == -1)
	{
		free_null (1, &abs_path);
		ms_exit(E_FORK, WEXITSTATUS(data->exit_code));
	}
	else if (data->pid == 0 && abs_path)
		exec_child(data, abs_path);
	if (abs_path)
		free_null (1, &abs_path);
}

bool	exec_program(t_data *data)
{
	char		*abs_path;
	int			err;
	DIR			*dir;

	data->pid = 1;
	dir = opendir(data->argv[0]);
	err = errno;
	if (dir)
		closedir(dir);
	if (err == 0)
		return (exec_error_fork(data, ft_strdup(data->argv[0]), true));
	abs_path = exec_get_path(data, data->argv[0]);
	if (!abs_path)
		return (exec_error_fork(data, data->argv[0], false));
	else
		exec_create_fork(data, abs_path);
	exec_close_pipes(data);
	exec_set_flags(data);
	return (true);
}
