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
	data->flags->bracket = false;
	data->flags->bracket2 = false;
	data->counter_pipes = 0;
	data->fd_i = 0;
}

bool count_pipes(t_data *data, char *cmd)
{
	int i;

	i = 0;
	data->counter_pipes = 0;
	while (cmd[i] && cmd[i + 1] && ft_strncmp(cmd + i, "&&", 2) && ft_strncmp(cmd + i, "||", 2))
	{
		if (cmd[i] && cmd[i] == '|')
			data->counter_pipes++;
		i++;
	}
	if (i)
		return (true);
	return (false);
}

static char last_char(char *str)
{
	int i;
	int pos_bracket;
	
	i = 0;
	pos_bracket = 0;
	while (str[i])
	{
		// if (str[i] == ')')
		// 	pos_bracket = i;
		i++;
	}
	return(str[i - 1]);
}

static void prio(t_data *data)
{
	int i;

	i = 0;
	data->flags->bracket = true;
	if ((data->flags->and && data->exit_status) || (data->flags->or && !data->exit_status))
	{
		data->argv[0] = NULL;
		while (data->cmd[i] != ')')
			data->cmd++;
		while (data->cmd[i] == ')')
			data->cmd++;
	}
	else
	{
		while (data->argv[i])
		{
			if (last_char(data->argv[i]) == ')')
				data->flags->bracket = false;
			data->argv[i] = ft_strtrim(data->argv[i], "()");
			i++;
		}
	}
}

static void	prompt(t_data *data)
{
	bool	left;
	char	*cmd;
	char	*cmd_buf;
	char	*cmd_rl;
	char	*cmd_split;
	int		i = 0;

	left = true;
	cmd = NULL;
	cmd_rl = readline(data->prompt);
	cmd_buf = ft_strdup(cmd_rl);
	free (cmd_rl);
	if (!cmd_buf)
		cmd_buf = ft_strdup("exit");
	if (cmd_buf && cmd_buf[0] != '\n')
	{
		history(data, cmd_buf);
		if (!check_syntax(data, cmd_buf))
			return ;
		cmd = pre_parse(data, cmd_buf);
		// printf("%s\n", cmd);
		// exit(0);
		// free (cmd_buf);
		if (count_pipes(data, cmd))
			open_pipes(data);
	}
	while (cmd && cmd[0] != '\0')
	{
		while (*cmd == ' ' || *cmd == ';')
			cmd++;
		cmd = split_quotes(data, cmd);
		expand_vars(data);
		if (data->argv[0] && (data->argv[0][0] == '(' || data->flags->bracket))
			prio(data);
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
	i = 0;
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
