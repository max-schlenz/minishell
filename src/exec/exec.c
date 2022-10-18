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

char	*get_path(t_data *data, char *cmd)
{
	int		i;
	char	*abs_path_tmp;
	char	*abs_path;
	char	*cmd_trim;

	i = 0;
	while (data->path && data->path[i])
	{
		if (ft_strlen(cmd) > 2 && ft_strchr(cmd + 2, '/'))
			return (NULL);
		abs_path_tmp = ft_strjoin(data->path[i], "/");
		cmd_trim = ft_strtrim(cmd, " ");
		abs_path = merge_str(2, abs_path_tmp, cmd_trim);
		if (!access(abs_path, F_OK) && check_path(abs_path))
			return (abs_path);
		free (abs_path);
		i++;
	}
	return (NULL);
}

static bool	exec_err(t_data *data, DIR *tmp, char *abs_path)
{
	if (data->flags->pipe)
	{
		exec_close_pipes(data);
		exec_set_flags(data);
	}
	perror("Error ");
	if (tmp)
	{
		closedir(tmp);
		data->exit_status = 126;
	}
	else
		data->exit_status = 127;
	free (abs_path);
	return (true);
}

void	exec_program_create_fork(t_data *data)
{
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
	if (!access(abs_path, F_OK))
		execve(abs_path, data->argv, data->envp);
	exit(0);
}

bool	exec_program(t_data *data)
{
	char		*abs_path;
	DIR			*dir;

	data->pid = 1;
	data->flags->error = false;
	abs_path = get_path(data, data->argv[0]);
	if (!abs_path && data->argv[0])
		abs_path = ft_strdup(data->argv[0]);
	dir = opendir(abs_path);
	if (data->flags->pipe)
		pipe(data->pipes->pipefd[data->fd_i]);
	if (!dir && (!access(abs_path, F_OK)))
		exec_program_create_fork(data);
	else
		data->flags->error = true;
	if (data->pid == 0)
		exec_program_child(data, abs_path);
	if (data->flags->error)
		return (exec_err(data, dir, abs_path));
	free (abs_path);
	if (dir)
		closedir(dir);
	exec_close_pipes(data);
	exec_set_flags(data);
	return (true);
}
