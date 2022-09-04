/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/04 13:14:43 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_data *data)
{
	data->cmd = NULL;
	lst_clear_btree(data);
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
			make_btree(data);
			while (data->counter_btree > 0)
			{
				if ((*data->btree)->left)
				{
					parse_args(data, (*data->btree)->left->value);
					if (!ft_strncmp((*data->btree)->value, "|", 2))
						data->flag_pipe = true;
					if (!builtins(data))
						exec_program(data);
				}
				if ((*data->btree)->right)
				{
					parse_args(data, ft_strtrim((*data->btree)->right->value, " "));
					if (!builtins(data))
						exec_program(data);
				}
				data->counter_btree--;
				if ((*data->btree)->left)
					(*data->btree) = (*data->btree)->left;
			}
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
