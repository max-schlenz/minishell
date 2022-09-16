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
	data->counter_pipes = 0;
	data->fd_i = 0;
}

bool count_pipes(t_data *data, char *cmd)
{
	int i;

	i = 0;
	data->counter_pipes = 0;
	while (cmd[i] && ft_strncmp(cmd + i, "&&", 2) && ft_strncmp(cmd + i, "||", 2))
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
	
	i = 0;
	while (str[i])
		i++;
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
		while (*data->cmd && *data->cmd != ')')
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
	char	*tmp_cmd;
	char	*tmp;
	int		i;

	left = true;
	// data->cmd = get_next_line(0);
	// data->cmd = ft_strtrim(data->cmd, "\n");
	data->cmd = readline(data->prompt);
	i = 0;
	if (!data->cmd)
		data->cmd = "exit";
	else if (data->cmd[0] && data->cmd[0] != '\n')
	{
		history(data);
		if (!check_syntax(data, data->cmd))
			return ;
		tmp_cmd = pre_parse(data, data->cmd);
		if (count_pipes(data, tmp_cmd))
			open_pipes(data);
		// data->cmd = find_wc(data, data->cmd);
	}
	while (tmp_cmd[i] && tmp_cmd[0] != '\0')
	{
		if (tmp_cmd[i] == ' ')
			i++;
		i = split_quotes(data, tmp_cmd, i);
		int k = 0;
		while(data->argv[k])
			printf("%s\n", data->argv[k++]);
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
		left = false;
	}
	data->flags->and = false;
	data->flags->or = false;
	free(data->cmd);
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
