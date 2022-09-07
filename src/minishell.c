/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/06 14:15:31 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_data *data)
{
	int i;

	data->cmd = NULL;
	free_array(data->argv);
	close(data->pipefd[0][0]);
	close(data->pipefd[0][1]);
	close(data->pipefd[1][0]);
	close(data->pipefd[1][1]);
	close(data->pipefd[2][0]);
	close(data->pipefd[2][1]);
	close(data->pipefd[3][0]);
	close(data->pipefd[3][1]);
	data->flag_pipe = 0;
}

static void	init_prompt(t_data *data)
{
	data->flag_error = false;
	data->flag_pipe = 0;
	data->counter_pipes = 0;
	pipe(data->pipefd[0]);
	pipe(data->pipefd[1]);
	pipe(data->pipefd[2]);
	pipe(data->pipefd[3]);
}

static void count_pipes(t_data *data)
{
	int i;

	i = 0;
	while (data->cmd[i])
	{
		if (data->cmd[i] == '|')
			data->counter_pipes++;
		i++;
	}
}

static void	prompt(t_data *data)
{
	add_history("ls | grep l");
	add_history("ls | grep l | grep m");
	add_history("ls | grep l | grep m | grep t | wc -c");
	data->cmd = readline("minishell >$ ");
	if (!data->cmd)
		data->cmd = ft_strdup("exit");
	else
	{
		add_history(data->cmd);
		count_pipes(data);
	}
	while (data->cmd && data->cmd[0] != '\0')
	{
		while (*data->cmd == ' ')
			*data->cmd ++;
		data->cmd = split_quotes(data, data->cmd);
		// if (*data->cmd == '|') //&& data->flag_pipe == -1)
		// 	data->flag_pipe = 0;
		if (*data->cmd == '|' || *data->cmd == ';')
			*data->cmd++;
		if (data->flag_error || !data->argv[0])
			continue;
		if (!builtins(data))
			exec_program(data);
		clear_buffers(data);
	}
}

static void	signals()
{
	struct sigaction	sa;
	
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	// signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	data = allocate_mem();
	signals();
	init_vars(data);
	parse_envp(data, envp);
	while (1)
	{
		init_prompt(data);
		prompt(data);
	}
}
