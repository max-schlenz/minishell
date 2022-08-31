/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 18:13:34 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_data	*data;

static void	clear_buffers(t_data *data)
{
	data->path = NULL;
	data->cmd = NULL;
}

static void	prompt(t_data *data)
{
	while (1)
	{
		data->cmd = readline("minishell >$ ");
		if (!data->cmd)									// << handles CTRL+D (EoT character)
			data->cmd = ft_strdup("exit");
		if (data->cmd && data->cmd[0] != '\0')
			add_history(data->cmd);
		clear_buffers(data);
	}
}

// https://stackoverflow.com/questions/71685072/return-readline-to-its-original-state-when-recieving-sigint
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

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	//sigaction(SIGUSR2, &sa, NULL);

	data = allocate_mem();
	//signal(SIGINT, signal_handler);
	parse_envp(data, envp);
	parse_path(data);
	// signal(SIGQUIT, SIG_IGN);
	prompt(data);
}
