/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/05 18:52:08 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_data *data)
{
	data->cmd = NULL;
}

static void	prompt(t_data *data)
{
	int i = 0;
	// data->r_pipe = pipe(data->pipes->pipefd);
	while (1)
	{
		data->flag_error = false;
		parse_path(data);
		data->cmd = readline("minishell >$ ");
		if (!data->cmd)
			data->cmd = ft_strdup("exit");
		while (data->cmd && data->cmd[0] != '\0')
		{
			add_history(data->cmd);
			while (*data->cmd == ' ')
				*data->cmd ++;
			data->cmd = split_quotes(data, data->cmd);
			if (*data->cmd == '|')
				*data->cmd++;
			if (data->flag_error)
				continue;
			if (!builtins(data))
				exec_program(data);
		}
		clear_buffers(data);
	}
}


int	main(int argc, char **argv, char **envp)
{
	struct sigaction	sa;
	t_data				*data;

	int i = 0;
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	data = allocate_mem();
	parse_envp(data, envp);
	prompt(data);
}
