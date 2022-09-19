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


static bool	strnum(char *str)
{
	int i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i] && (str[i] >= 48 && str[i] <= 57))
		i++;
	if (i < ft_strlen(str))
		return (false);
	return (true);
}

static int exit_code_thing(int exit_status)
{
	int i;
	int ret;
	
	i = 0;
	ret = 0;
	while (i < exit_status)
	{
		if (ret == 256)
			ret = 0;
		ret++;
		i++;
	}
	i = 0;
	while (i > exit_status)
	{
		if (ret == -1)
			ret = 255;
		ret--;
		i--;
	}
	return (ret);
}

bool	builtin_exit(t_data *data)
{
	if (data->argc > 1)
	{
		write(2, "Error: too many arguments\n", 27);
		data->exit_status = 1;
	}
	else if (data->argv[1] && strnum(data->argv[1]))
	{
		data->exit_status = ft_atoi(data->argv[1]);
		if (data->exit_status > 255)
			data->exit_status = exit_code_thing(data->exit_status);
	}
	if (data->argv[1] && !strnum(data->argv[1]))
	{
		write(2, "Error: numeric argument required\n", 34);
		data->exit_status = 2;
	}
	write(2, "exit", 5);
	free_array(data->argv);
	free (data->argv);
	cleanup(data, 0);
}

bool	builtin_environment(t_data *data)
{
	if (!ft_strncmp(data->argv[0], "exit", 5)) // || data->argv[0] == '\0')
		return (builtin_exit(data));
	else if (!ft_strncmp(data->argv[0], "cd", 3))
		return (builtin_cd(data));
	else if (!ft_strncmp(data->argv[0], "export", 7))
		return (builtin_export(data, NULL));
	else if (!ft_strncmp(data->argv[0], "unset", 6))
		return (builtin_unset(data));
	else if (!ft_strncmp(data->argv[0], "color", 6))
		return (builtin_color(data, NULL));
	else if (!ft_strncmp(data->argv[0], "history", 8))
		return (builtin_history(data));
	return (false);
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

void	pipes(t_data *data)
{
	if (data->fd_i == 0)
	{
		// fprintf(data->debug, "pipe fd 0 write <-> stdout\n");
		dup2(data->pipes->pipefd[0][1], 1);								//stdout <-> fd 0 write
	}
	else if (data->fd_i < data->counter_pipes)
	{
		// fprintf(data->debug, "pipe fd %d read <-> stdin\n", data->fd_i - 1);
		dup2(data->pipes->pipefd[data->fd_i - 1][0], 0);				//stdin <-> fd 0 read
		// fprintf(data->debug, "pipe fd %d write <-> stdout\n", data->fd_i);
		dup2(data->pipes->pipefd[data->fd_i][1], 1);			 		//stdout <-> fd 1 write
	}
	else
	{
		// fprintf(data->debug, "pipe fd %d read <-> stdin\n", data->fd_i - 1);
		dup2(data->pipes->pipefd[data->fd_i - 1][0], 0);				//stdin <-> fd 3 read
	}
}

static void	dbg(t_data *data)
{
	int i;
	
	i = 0;
	fprintf(data->debug, "\n\n------------------\n\n");
	while (data->argv[i])
	{
		fprintf(data->debug, "argv[%d] = %s\n", i, data->argv[i]);
		i++;
	}
	// fprintf(data->debug, "\ndata->cmd = %s\n", data->cmd);
	fprintf(data->debug, "------------------\n");
	fprintf(data->debug, "FLAGS:\n");
	fprintf(data->debug, "redir_out    : %d\n", data->flags->redir_out);
	fprintf(data->debug, "redir_in     : %d\n", data->flags->redir_in);
	fprintf(data->debug, "heredoc      : %d\n", data->flags->heredoc);
	fprintf(data->debug, "redir_append : %d\n", data->flags->redir_append);
	fprintf(data->debug, "pipe         : %d\n\n", data->flags->pipe);
	fprintf(data->debug, "and          : %d\n", data->flags->and);
	fprintf(data->debug, "or           : %d\n", data->flags->or);
	fprintf(data->debug, "VARS:\n");
	fprintf(data->debug, "data->fd_i            : %d\n", data->fd_i);
	fprintf(data->debug, "data->counter_pipes   : %d\n", data->counter_pipes);
	fprintf(data->debug, "data->file_name       : %s\n", data->file_name);
	fprintf(data->debug, "data->file_name2      : %s\n", data->file_name2);
	fprintf(data->debug, "data->exit_status (p) : %d\n", data->exit_status);
	fprintf(data->debug, "------------------\n");
}

void	redirs_pipes(t_data *data)
{
	int		fd;
	int		heredoc_fd[2];
	char	*heredoc_tmp;

	if (data->counter_pipes > 0 && data->flags->pipe)
		pipes(data);
	if (data->flags->redir_out && data->flags->redir_in)
	{
		fd = open(data->file_name2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (data->flags->redir_out)
	{
		fd = open(data->file_name2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->heredoc)
	{
		pipe(heredoc_fd);
		heredoc_tmp = ft_strdup("42");
		while (ft_strncmp(data->heredoc_delim, heredoc_tmp, ft_strlen(data->heredoc_delim)))
		{
			free(heredoc_tmp);
			heredoc_tmp = get_next_line(0);
			heredoc_tmp[ft_strlen(heredoc_tmp)] = '\0';
			if (ft_strncmp(data->heredoc_delim, heredoc_tmp, ft_strlen(heredoc_tmp)))
				write(heredoc_fd[1], heredoc_tmp, ft_strlen(heredoc_tmp));
		}
		free(heredoc_tmp);
		close(heredoc_fd[1]);
		dup2(heredoc_fd[0], STDIN_FILENO);
		close(heredoc_fd[0]);
	}
	if (data->flags->redir_append)
	{
		fd = open(data->file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (data->flags->redir_in)
	{
		fd = open(data->file_name, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

bool	exec_program(t_data *data)
{
	pid_t		pid;
	char		*abs_path;
	bool		error;
	DIR			*tmp;

	data->exit_code = 0;
	pid = 1;
	error = false;
	data->flags->error = false;
	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->argv[0]);
	// data->debug = fopen("debug", "a+");
	tmp = opendir(abs_path);
	if (!tmp && !access(abs_path, F_OK))
		pid = fork();
	else
		error = true;
	if (pid == -1)
		ms_exit(2, WEXITSTATUS(data->exit_code));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		// if (data->flags->debug)
		// 	dbg(data);
		redirs_pipes(data);
		// fclose(data->debug);
		if (!builtin_print(data))
		{
			if (!access(abs_path, F_OK))
				execve(abs_path, data->argv, data->envp);
		}
		exit(0);
	}
	if (error)
	{
		if (tmp)
			data->exit_status = 126;
		else
			data->exit_status = 127;
		perror("Error ");
		free (abs_path);
		closedir(tmp);
		return (true);
	}
	free (abs_path);
	closedir(tmp);
	waitpid(pid, &data->exit_code, 0);
	reset_pipes_flags(data);
	return (true);
}

void	reset_pipes_flags(t_data *data)
{
	if (data->counter_pipes > 0 && data->fd_i != data->counter_pipes)
		close(data->pipes->pipefd[data->fd_i][1]);
	data->fd_i++;
	if (data->flags->redir_out)
		data->flags->redir_out = false;
	if (data->flags->redir_in)
		data->flags->redir_in = false;
	if (data->flags->redir_append)
		data->flags->redir_append = false;
	if (data->flags->heredoc)
		data->flags->heredoc = false;
	if (data->flags->pipe && (data->flags->redir_out || data->flags->redir_in))
		data->flags->pipe = false;
	data->exit_status = WEXITSTATUS(data->exit_code);
}
