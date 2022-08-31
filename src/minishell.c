/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 17:14:33 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_msdata *data)
{
	data->path = NULL;
	data->cmd = NULL;
}

// https://stackoverflow.com/questions/71685072/return-readline-to-its-original-state-when-recieving-sigint
static void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		printf("\033[A\nminishell >$ ");
	}
}

static void	prompt(t_msdata *data, char **envp)
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

int	main(int argc, char **argv, char **envp)
{
	t_msdata	*data;
	int i = 0;

	data = allocate_mem();
	signal(SIGINT, signal_handler);
	parse_envp(data, envp);
	parse_path(data);
	// signal(SIGQUIT, SIG_IGN);
	prompt(data, envp);
}
