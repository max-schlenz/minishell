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
	while (data->path && data->path[i])
	{
		if (ft_strlen(cmd) > 2 && ft_strchr(cmd + 2, '/'))
			break ;
		abs_path_tmp = ft_strjoin(data->path[i], "/");
		cmd_trim = ft_strtrim(cmd, " ");
		abs_path = merge_str(2, abs_path_tmp, cmd_trim);
		if (!access(abs_path, F_OK) && exec_check_path(abs_path))
			return (abs_path);
		free (abs_path);
		i++;
	}
	return (ft_strdup(cmd));
}

static bool	exec_error_fork(t_data *data, char *abs_path, bool isdir, DIR *dir)
{
	char	*msg;

	if (dir)
		closedir(dir);
	if (isdir)
	{
		msg = E_EXEC_ISDIR;
		data->exit_status = 126;
	}
	else
	{
		msg = E_EXEC_NOTFOUND;
		data->exit_status = 127;
	}
	if (data->flags->pipe)
	{
		exec_close_pipes(data);
		exec_set_flags(data);
		data->flags->pipe = false;
	}
	write(2, "Error: ", 8);
	write(2, abs_path, ft_strlen(abs_path));
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (free (abs_path), true);
}

static void	exec_child(t_data *data, char *abs_path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	exec_redirs_pipes(data);
	if (!access(abs_path, F_OK))
	{
		if (execve(abs_path, data->argv, data->envp) == -1)
			exec_error_fork(data, abs_path, false, NULL);
	}
	exit(data->exit_status);
}

void	exec_create_fork(t_data *data, char *abs_path)
{
	signal(SIGINT, SIG_IGN);
	data->pid = fork();
	if (!data->pid && abs_path)
		exec_child(data, abs_path);
	else if (data->pid == -1)
		ms_exit(2, WEXITSTATUS(data->exit_code));
}

bool	exec_program(t_data *data)
{
	char		*abs_path;
	DIR			*dir;

	data->pid = 1;
	dir = NULL;
	abs_path = exec_get_path(data, data->argv[0]);
	if (!abs_path)
		return (exec_error_fork(data, abs_path, true, dir));
	dir = opendir(abs_path);
	if (dir)
		return (exec_error_fork(data, abs_path, true, dir));
	if (abs_path[0] && abs_path[1] && abs_path[1] == '/' && dir)
		return (exec_error_fork(data, abs_path, true, dir));
	if (!access(abs_path, F_OK) && !dir)
	{
		if (data->flags->pipe)
			pipe(data->pipes->pipefd[data->fd_i]);
		exec_create_fork(data, abs_path);
	}
	else
		return (exec_error_fork(data, abs_path, false, dir));
	free (abs_path);
	exec_close_pipes(data);
	exec_set_flags(data);
	return (true);
}
