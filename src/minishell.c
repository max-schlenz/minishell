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
	data->argc = 0;
	if (data->cmd)
		free (data->cmd);
	data->cmd = NULL;
	if (data->file_name)
		free (data->file_name);
	data->file_name = NULL;
	if (data->file_name2)
		free (data->file_name2);
	data->file_name2 = NULL;
	if (data->hdoc.delim)
		free (data->hdoc.delim);
	data->hdoc.delim = NULL;
	// if (data->flags->pipe)
	// 	close_pipes(data);
	rm_tmp_files(data);
	data->flags->and = false;
	data->flags->or = false;
	data->flags->pipe = 0;
}

static void	init_prompt(t_data *data)
{
	data->flags->error = false;
	data->flags->pipe = false;
	data->flags->redir_out = false;
	data->flags->redir_append = false;
	data->flags->redir_in = false;
	data->flags->heredoc = false;
	data->flags->heredoc_begin = false;
	data->flags->and = false;
	data->flags->or = false;
	data->flags->pipe = false;
	data->flags->bracket = false;
	data->flags->prio = false;
	data->counter_pipes = 0;
	data->parser.array_index = 0;
	data->parser.arg_start = 0;
	data->argc = 0;
	data->fd_i = 0;
	data->pid = 1;
}

bool	count_pipes(t_data *data, char *cmd)
{
	int	i;

	i = 0;
	data->counter_pipes = 0;
	while (cmd[i]
		&& ft_strncmp(cmd + i, "&&", 2) && ft_strncmp(cmd + i, "||", 2))
	{
		if (cmd[i] && cmd[i] == '|')
			data->counter_pipes++;
		i++;
	}
	if (i)
		return (true);
	return (false);
}

static char	last_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (str[i - 1]);
}

void	prio(t_data *data, char *cmd, int *i)
{
	int		j;
	char	*tmp;

	j = 0;
	data->flags->bracket = true;
	if ((data->flags->and && data->exit_status)
		|| (data->flags->or && !data->exit_status))
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

void	wait_for_childs(t_data *data)
{
	while (wait(&data->exit_code) > 0)
		data->exit_status = WEXITSTATUS(data->exit_code);
		// continue ;
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	data = allocate_mem();
	signals(false);
	init_vars(data);
	if (!*envp)
	{
		data->flags->noenv = true;
		envp = ft_calloc(1, sizeof(char **));
		envp[0] = NULL;
	}
	parse_envp(data, envp);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		init_prompt(data);
		data->exit_status = prompt(data, argv[2], 1);
		clear_buffers(data);
		cleanup(data, 0);
	}
	read_cfg(data);
	while (1)
	{
		init_prompt(data);
		prompt(data, NULL, 0);
		clear_buffers(data);
	}
}
