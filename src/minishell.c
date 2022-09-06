/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/06 12:32:45 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_data *data)
{
	data->cmd = NULL;
	if (data->argv)
	{
		while (*data->argv)
			free(*data->argv++);
	}
}

static void	init_prompt(t_data *data)
{
	data->flag_error = false;
	data->flag_pipe = 0;
}

static void	prompt(t_data *data)
{
	data->r_pipe = pipe(data->pipes->pipefd);
	data->cmd = readline("minishell >$ ");
	if (!data->cmd)
		data->cmd = ft_strdup("exit");
	else	
		add_history(data->cmd);
	while (data->cmd && data->cmd[0] != '\0')
	{
		while (*data->cmd == ' ')
			*data->cmd ++;
		data->cmd = split_quotes(data, data->cmd);
		if (*data->cmd == '|' && data->flag_pipe == 0)
			data->flag_pipe = 1;
		if (*data->cmd == '|' || *data->cmd == ';')
			*data->cmd++;
		if (data->flag_error || !data->argv[0])
			continue;
		if (!builtins(data))
			exec_program(data);
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
		clear_buffers(data);
	}
}
