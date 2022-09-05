data->r_pipe = pipe(data->pipes->pipefd);

data->pipes = ft_calloc(1, sizeof(t_pipes));

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
		dup2(data->pipes->pipefd[1], 1);
		close(data->pipes->pipefd[0]);
		execve(abs_path, data->args, data->envp);
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(data->pipes->pipefd[0], 0);
		close(data->pipes->pipefd[1]);
		execve(abs_path, data->args, data->envp);
		// execve("/usr/bin/grep", blub, data->envp);
	}
	close(data->pipes->pipefd[1]);
	waitpid(pid, NULL, 0);
	exit_code = WEXITSTATUS(pid);
}