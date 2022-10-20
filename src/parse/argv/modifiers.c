/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modifiers.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:19:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/20 15:48:20 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	parse_string(t_data *data, char *cmd, int i, bool end)
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

bool	parse_or(t_data *data, char *cmd, int *i, int start_args)
{
	data->flags->or = true;
	if ((*i) != start_args)
		return (true);
	data->flags->and = false;
	(*i) += 3;
	data->fd_i = 0;
	wait_for_childs(data);
	// count_pipes(data, cmd + (*i));
	return (true);
}

bool	parse_and(t_data *data, char *cmd, int *i, int start_args)
{
	data->flags->and = true;
	if ((*i) != start_args)
		return (true);
	data->flags->or = false;
	data->flags->pipe = false;
	(*i) += 3;
	data->fd_i = 0;
	wait_for_childs(data);
	// count_pipes(data, cmd + (*i));
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
