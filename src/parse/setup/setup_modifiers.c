/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_modifiers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:19:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/29 12:44:14 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	setup_argv_parse_arg(t_data *data, char *cmd, int i, bool end)
{
	free(data->argv[data->parser.array_index]);
	data->argv[data->parser.array_index] = NULL;
	data->argv[data->parser.array_index]
		= ft_substr(cmd, data->parser.arg_start, i - data->parser.arg_start);
	data->argc = data->parser.array_index;
	data->parser.array_index++;
	if (!end)
		data->parser.arg_start = i + 1;
	return (true);
}

	// data->flags->redir_out = false;
	// data->flags->redir_append = false;
	// data->flags->redir_in = false;
	// while (waitpid(-1, &data->exit_code, 0) != -1)
	// {
	// 	if (WIFEXITED(data->exit_code))
	// 		data->exit_status = WEXITSTATUS(data->exit_code);
	// }
bool	setup_argv_is_or(t_data *data, int *i, int start_args)
{
	if ((*i) != start_args)
		return (true);
	data->flags->or = true;
	data->flags->and = false;
	data->flags->pipe = false;
	(*i) += 3;
	data->fd_i = 0;
	exec_wait_for_childs(data);
	init_prompt(data);
	data->counter_pipes = 0;
	return (true);
}

	// data->file_name2 = NULL;
	// data->flags->redir_out = false;
	// data->flags->redir_append = false;
	// data->flags->redir_in = false;
	// while (waitpid(-1, &data->exit_code, 0) != -1)
	// {
	// 	if (WIFEXITED(data->exit_code))
	// 		data->exit_status = WEXITSTATUS(data->exit_code);
	// }
bool	setup_argv_is_and(t_data *data, int *i, int start_args)
{
	if ((*i) != start_args)
		return (true);
	data->flags->and = true;
	data->flags->or = false;
	data->flags->pipe = false;
	(*i) += 3;
	data->fd_i = 0;
	exec_wait_for_childs(data);
	init_prompt(data);
	data->counter_pipes = 0;
	return (true);
}

bool	setup_argv_is_pipe(t_data *data, int *i)
{
	data->flags->pipe = true;
	(*i) += 2;
	return (true);
}

bool	setup_argv_is_redir_out(t_data *data, char *cmd, int *i)
{
	(*i) += 3;
	data->flags->redir_append = true;
	setup_filenames(data, i, cmd, 2);
	if ((*i) < ft_strlen(cmd))
	{
		(*i)++;
		if (cmd[*i] && cmd[*i] == '|')
			setup_pipe(data, cmd, i);
	}
	return (true);
}
