/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/04 17:27:00 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_data *data)
{
	data->cmd = NULL;
}

static void	prompt(t_data *data)
{
	while (1)
	{
		parse_path(data);
		data->cmd = readline("minishell >$ ");
		if (!data->cmd)									// << handles CTRL+D (EoT character)
			data->cmd = ft_strdup("exit");
		if (data->cmd && data->cmd[0] != '\0')
		{
			add_history(data->cmd);
			data->cmd_split = str_split(data->cmd, "|><");
			int i = 0;
			while(data->cmd_split[i])
			{
				printf("%s\n", data->cmd_split[i]);
				i++;
			}
			parse_args(data, data->cmd);
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
