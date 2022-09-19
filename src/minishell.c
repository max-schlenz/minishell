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

	if (data->cmd)
		free (data->cmd);
	data->cmd = NULL;
	if (data->file_name)	
		free (data->file_name);
	data->file_name = NULL;
	if (data->file_name2)
		free (data->file_name2);
	data->file_name2 = NULL;
	if (data->heredoc_delim)
		free (data->heredoc_delim);
	data->heredoc_delim = NULL;
	close_pipes(data);
	data->flags->pipe = 0;
}

static void	init_prompt(t_data *data)
{
	data->flags->debug = false;
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
	data->flags->prio = false;
	data->counter_pipes = 0;
	data->argc = 0;
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

static void prio(t_data *data, char *cmd, int *i)
{
	int 	j;
	char	*tmp;

	j = 0;
	data->flags->bracket = true;
	if ((data->flags->and && data->exit_status) || (data->flags->or && !data->exit_status))
	{
		data->flags->prio = true;
		free_array(data->argv);
		free(data->argv);
		data->argv = ft_calloc(1, sizeof(char **));
		data->argv[0] = NULL;
		while (cmd[*i] && cmd[*i] != ')')
			(*i)++;
	}
	else
	{
		while (data->argv[j])
		{
			if (last_char(data->argv[j]) == ')')
				data->flags->bracket = false;
			tmp = ft_strtrim(data->argv[j], "()");
			if (data->argv[j])
				free (data->argv[j]);
			data->argv[j] = tmp;
			j++;
		}
	}
}

static int	prompt(t_data *data, char *cmd, int flag)
{
	bool	left;
	char	*tmp_cmd;
	int		i;

	left = true;
	data->cmd = NULL;
	if (flag)
		data->cmd = ft_strdup(cmd);
	else
		data->cmd = readline(data->prompt);
	i = 0;
	if (!data->cmd)
		data->cmd = ft_strdup("exit");
	if (data->cmd[0] && data->cmd[0] != '\n')
	{
		history(data);
		if (!check_syntax(data) || !syntax_err(data) || !check_syntax_first_char(data))
			return (data->exit_status);
		tmp_cmd = pre_parse(data, data->cmd);
		if (count_pipes(data, tmp_cmd))
			open_pipes(data);
	}
	else
		return (0);
	while (tmp_cmd[i] && tmp_cmd[0])
	{
		while (tmp_cmd[i] == ' ' || tmp_cmd[i] == ';')
			i++;
		i = split_quotes(data, tmp_cmd, i);
		if (!tmp_cmd[i - 1])
		{
			free_array(data->argv);
			free(data->argv);
			break ;
		}
		expand_vars(data);
		if (data->argv[0] && (data->argv[0][0] == '(' || data->flags->bracket))
			prio(data, tmp_cmd, &i);
		if (data->flags->error || !data->argv[0])
		{
			free_array(data->argv);
			free(data->argv);
			continue;
		}
		if ((left)
		||	(!data->flags->and && !data->flags->or) 
		||	(data->flags->and && !data->exit_status) 
		||	(data->flags->or && data->exit_status))
		{
			if (!builtin_environment(data))
				exec_program(data);
		}
		left = false;
		free_array(data->argv);
		free(data->argv);
	}
	data->flags->and = false;
	data->flags->or = false;
	free(tmp_cmd);
	tmp_cmd = NULL;
	return (data->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	data = allocate_mem();
	signals();
	init_vars(data);
	read_config(data);
	parse_envp(data, envp);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
  	{
		init_prompt(data);
    	data->exit_status = prompt(data, argv[2], 1);
		clear_buffers(data);
    	cleanup(data, 0);
  	}
	while (1)
	{
		init_prompt(data);
		prompt(data, NULL, 0);
		clear_buffers(data);
	}
}
