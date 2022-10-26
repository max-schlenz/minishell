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

// if (data->flags->pipe)
// 	close_pipes(data);
void	clear_buffers(t_data *data)
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
	rm_tmp_files(data);
	data->flags->and = false;
	data->flags->or = false;
	data->flags->pipe = false;
}

void	init_prompt(t_data *data)
{
	data->fd_stdin = dup(0);
	data->fd_stdout = dup(1);
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
	data->cd.oldpwd_err = false;
	data->counter_pipes = 0;
	data->parser.array_index = 0;
	data->parser.arg_start = 0;
	data->argc = 0;
	data->fd_i = 0;
	data->pid = 1;
	data->flags->f_esc = false;
	free_filenames(data);
}

bool	count_pipes(t_data *data, char *cmd)
{
	int		i;
	char	buf;

	i = 0;
	buf = 0;
	data->counter_pipes = 0;
	while (cmd[i]
		&& ft_strncmp(cmd + i, "&&", 2) && ft_strncmp(cmd + i, "||", 2))
	{
		if (cmd[i] && cmd[i] == '|' && buf && buf != '\\')
			data->counter_pipes++;
		buf = cmd[i];
		i++;
	}
	if (i)
		return (true);
	return (false);
}

static void	switches(t_data *data, int argc, char **argv)
{
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		init_prompt(data);
		data->exit_status = prompt(data, argv[2], 1);
		clear_buffers(data);
		cleanup(data, 0);
	}
	if (argc >= 2 && !ft_strncmp(argv[1], "-r", 3))
		unlink(CFG);
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	data = allocate_mem();
	signals(false);
	init_vars(data);
	parse_envp(data, envp);
	switches(data, argc, argv);
	read_cfg(data);
	while (1)
	{
		init_prompt(data);
		prompt(data, NULL, 0);
		clear_buffers(data);
	}
}
