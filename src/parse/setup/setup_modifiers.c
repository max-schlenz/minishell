/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_modifiers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:19:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 12:51:33 by tdehne           ###   ########.fr       */
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

// bool	setup_argv_is_redir_out(t_data *data, char *cmd, int *i)
// {
// 	(*i) += 3;
// 	data->flags->redir_append = true;
// 	//setup_filenames(data, i, cmd, 2);
// 	return (true);
// }

bool	setup_argv_set_redir_flags(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& !ft_strncmp(cmd + (*i), ">>", 2))
	{
		(*i) += 3;
		data->flags->redir_append = true;
		return (setup_all_filenames(data, i, cmd, 2));
	}
	else if (cmd[*i] == '>')
	{
		(*i) += 2;
		data->flags->redir_out = true;
		return (setup_all_filenames(data, i, cmd, 1));
	}
	else if (cmd[*i] == '<')
	{
		(*i) += 2;
		data->flags->redir_in = true;
		return (setup_all_filenames(data, i, cmd, 0));
	}
	return (false);
}
