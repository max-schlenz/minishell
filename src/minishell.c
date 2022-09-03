/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 13:33:16 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_data	*data;

static void	clear_buffers(t_data *data)
{
	data->cmd = NULL;
}

static void	prompt(t_data *data, char **envp)
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
			parse_args(data);
			lol(data);
			exit(0);
			if (!builtins(data))
				exec_program(data, envp);
		}
		clear_buffers(data);
	}
}

static void	signal_handler(int sig, siginfo_t *info, void *context)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **envp)
{
	int i = 0;
	struct sigaction	sa;
	t_data	*data;

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	data = allocate_mem();
	parse_envp(data, envp);
	prompt(data, envp);
}
