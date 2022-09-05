/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/05 14:33:45 by mschlenz         ###   ########.fr       */
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
		parse_path(data);
		data->cmd = readline("minishell >$ ");
		if (!data->cmd)
			data->cmd = ft_strdup("exit");
		if (data->cmd && data->cmd[0] != '\0')
		{
			add_history(data->cmd);
			if (!split_quotes(data, data->cmd))
				continue ;
			// parse_args(data, i);
			// if (!builtins(data))
			// 	exec_program(data);
			// i = 0;
			// data->cmd_split = str_split(data->cmd, "|><");
			// while(data->cmd_split[i])
			// {
			// 	data->cmd_split[i] = ft_strtrim(data->cmd_split[i], " ");
			// 	// printf("%s\n", data->cmd_split[i]);
			// 	i++;
			// }
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
