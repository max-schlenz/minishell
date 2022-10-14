/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modifiers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:19:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 21:52:50 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	parse_string(t_data *data, char *cmd, int i, bool end)
{
	free(data->argv[data->parser.array_index]);
	data->argv[data->parser.array_index] = NULL;
	if (cmd && cmd[data->parser.arg_start] == '\\')
		data->parser.arg_start++;
	data->argv[data->parser.array_index]
		= ft_substr(cmd, data->parser.arg_start, i - data->parser.arg_start);
	data->argc = data->parser.array_index;
	data->parser.array_index++;
	if (!end)
		data->parser.arg_start = i + 1;
	return (true);
}

bool	parse_or(t_data *data, char *cmd, int *i, int start_args)
{
	if ((*i) != start_args)
		return (true);
	data->flags->and = false;
	data->flags->or = true;
	(*i) += 3;
	data->fd_i = 0;
	count_pipes(data, cmd + (*i));
	wait_for_childs(data);
	return (true);
}

bool	parse_and(t_data *data, char *cmd, int *i, int start_args)
{
	if ((*i) != start_args)
		return (true);
	data->flags->and = true;
	data->flags->or = false;
	data->flags->pipe = false;
	(*i) += 3;
	data->fd_i = 0;
	count_pipes(data, cmd + (*i));
	wait_for_childs(data);
	return (true);
}

bool	parse_pipes(t_data *data, int *i)
{
	data->flags->pipe = true;
	(*i) += 2;
	return (true);
}

bool	parse_redir_out(t_data *data, char *cmd, int *i)
{
	(*i) += 3;
	data->flags->redir_append = true;
	set_filenames(data, i, cmd, 2);
	return (true);
}
