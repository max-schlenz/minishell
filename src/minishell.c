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
	// free_array(data->argv);
	// free(data->argv);
	// data->cmd = NULL;
	close_pipes(data);
	data->flags->pipe = 0;
}

static void	init_prompt(t_data *data)
{
	data->flags->debug = true;
	data->flags->error = false;
	data->flags->pipe = false;
	data->flags->redir_out = false;
	data->flags->redir_append = false;
	data->flags->redir_in = false;
	data->flags->heredoc = false;
	data->flags->and = false;
	data->flags->or = false;
	data->flags->pipe = false;
	data->counter_pipes = 0;
	data->fd_i = 0;
}

static bool count_pipes(t_data *data)
{
	int i;

	i = 0;
	while (data->cmd[i])
	{
		if (!ft_strncmp(data->cmd + i, "||", 2))
			i += 2;
		if (data->cmd[i] && data->cmd[i] == '|')
			data->counter_pipes++;
		i++;
	}
	if (i)
		return (true);
	return (false);
}

static void	prompt(t_data *data)
{
	bool	left;
	char	*cmd;

	left = true;
	data->cmd = readline(data->prompt);
	if (!data->cmd)
		data->cmd = "exit";
	else if (data->cmd[0] && data->cmd[0] != '\n')
	{
		history(data);
		if (count_pipes(data))
			open_pipes(data);
		if (!check_syntax(data))
			return ;
		data->cmd = pre_parse(data->cmd);
		data->cmd = find_wc(data, data->cmd);
	}
	while (data->cmd && data->cmd[0] != '\0')
	{
		while (*data->cmd == ' ')
			data->cmd++;
		// printf("%s\n", data->cmd);
		// exit(0);
		data->cmd = split_quotes(data, data->cmd);
		expand_vars(data);
		// int k = 0;
		// while (data->argv[k])
		// exit(0);
		// if (*data->cmd == ';')
		// 	data->cmd++;
		if (data->flags->error || !data->argv[0])
			continue;
		if ((left)
		||	(!data->flags->and && !data->flags->or) 
		||	(data->flags->and && !data->exit_status) 
		||	(data->flags->or && data->exit_status))
		{
			if (!builtin_environment(data))
				exec_program(data);
		}
		free_array(data->argv);
		free(data->argv);
		left = !left;
	}
	data->flags->and = false;
	data->flags->or = false;
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	data = allocate_mem();
	signals();
	init_vars(data, argv);
	read_config(data);
	parse_envp(data, envp);
	while (1)
	{
		init_prompt(data);
		prompt(data);
		clear_buffers(data);
	}
}
