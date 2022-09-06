#include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/uio.h>
# include <stdio.h>
# include <stddef.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <stdlib.h>

typedef struct s_data
{
	char	**envp;
}	t_data;

char *lol(char *str)
{
	str++;
	return (str);
}

void	parse_envp(char **envp)
{
	int		i;
	char	**test;

	i = 0;
	while (envp[i])
		i++;
	test = malloc(i + 1 * sizeof(char *));
	test = envp;
	while (*test)
	{
		printf("%s\n", *(test++));
	}
	/*while (i < data->counter_envv)
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}*/
	// data->envp[i] = NULL;
}
// void piping(char *str, int *fd, int flag)
// {
// 	int	pid;
// 	pipe(fd);
// 	pid1 = fork();
// 	if (pid == 0)
// 	{
// 		dup2(data->pipes->pipefd[1], 1);
// 		close(data->pipes->pipefd[0]);
// 		execve(abs_path, data->args, data->envp);
// 	}
// 	pid2 = fork();
// 	if (pid == 0)
// 	{
// 		dup2(data->pipes->pipefd[0], 0);
// 		close(data->pipes->pipefd[1]);
// 		execve(abs_path, data->args, data->envp);
// 		// execve("/usr/bin/grep", blub, data->envp);
// 	}
// 	close(data->pipes->pipefd[1]);
// 	waitpid(pid, NULL, 0);
// }

int main(int argc, char **argv, char **envp)
{
	int	fd[2];
	char **blub = (char **)malloc(sizeof(char *) * 4);
	blub[0] = "grep";
	blub[1] = "x";
	blub[2] = "lol";
	blub[3] = NULL;
	char **blub2 = (char **)malloc(sizeof(char *) * 3);
	blub2[0] = "grep";
	blub2[1] = "void";
	blub2[2] = NULL;
	char **blub3 = (char **)malloc(sizeof(char *) * 3);
	blub3[0] = "wc";
	blub3[1] = "-l";
	blub3[2] = NULL;
	// while (*str)
	// {
	// 	piping(*str, fd, flag);
	// 	str++;
	// }
	int	pid1;
	int	pid2;
	int	pid3;

	pipe(fd);
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		execve("/usr/bin/grep", blub, envp);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[1], 1);
		dup2(fd[0], 0);
		close(fd[1]);
		//exceclp("grep", "grep", "rtt", NULL);
		execve("/usr/bin/grep", blub2, envp);
	}
	/*pid3 = fork();
	if (pid3 == 0)
	{
		dup2(fd[0], 0);
		//dup2(1, 0);
		close(fd[0]);
		close(fd[1]);
		execve("/usr/bin/wc", blub3, envp);
		// execve("/usr/bin/grep", blub, data->envp);
	}*/
	close(fd[1]);
	close(fd[0]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	
}