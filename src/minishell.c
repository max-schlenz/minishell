/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 17:16:36 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_btree(t_data *data)
{
	t_btree	*buf_list;

	if (!data->btree)
		return ;
	while (*data->btree)
	{
		buf_list = (*data->btree)->left;
		free (*data->btree);
		(*data->btree) = buf_list;
	}
	data->counter_btree = 0;
}

static void	clear_buffers(t_data *data)
{
	data->cmd = NULL;
	clear_btree(data);
}

// static int	count_modifiers(t_data *data)
// {
// 	int i;

// 	i = 0;
// 	while (data->cmd[i])
// 	{
// 		if (data->cmd[i] == '|')
			
// 		i++;
// 	}
// }

static void	prompt(t_data *data, char **envp)
{
	int		r_pipe;

	// data->pipes.rdbuf = ft_calloc(4096, sizeof(char));
	// data->r_pipe = pipe(data->pipes.pipefd);
	while (1)
	{
		parse_path(data);
		data->cmd = readline("minishell >$ ");
		if (!data->cmd)									// << handles CTRL+D (EoT character)
			data->cmd = ft_strdup("exit");
		if (data->cmd && data->cmd[0] != '\0')
		{
			add_history(data->cmd);
			// count_modifiers(data);
			lol(data);
			while (data->counter_btree > 0)
			{
				if ((*data->btree)->left)
				{
					data->cmd = (*data->btree)->left->value;
					parse_args(data);
					if (!ft_strncmp((*data->btree)->value, "|", 2))
						data->flag_pipe = true;
					if (!builtins(data))
						exec_program(data, envp);
				}
				if ((*data->btree)->right)
				{
					data->cmd = ft_strtrim((*data->btree)->right->value, " ");
					parse_args(data);
					if (!builtins(data))
						exec_program(data, envp);
				}
				data->counter_btree--;
				if ((*data->btree)->left)
					(*data->btree) = (*data->btree)->left;
			}
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
	// close(data->pipes.pipefd[0]);
	// close(data->pipes.pipefd[1]);
	parse_envp(data, envp);
	prompt(data, envp);
}
