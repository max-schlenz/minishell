/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/23 11:21:16 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	split_def(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& cmd[*i] == ' ' && cmd[(*i) + 1] && cmd[(*i) + 1] != ' ')
		parse_string(data, cmd, (*i), false);
}

static bool	split_iter(char *cmd, int *i)
{
	if (cmd[(*i)])
	{
		(*i)++;
		return (true);
	}
	else
		return (false);
}

static bool	split_parse(t_data *data, char *cmd, int *i, int start_args)
{
	int	flag_redir;

	flag_redir = 0;
	while (cmd[*i])
	{
		if (split_andor(data, cmd, i, start_args))
			return (true);
		flag_redir = split_redir(data, cmd, i);
		if (flag_redir == 1)
			return (true);
		if (flag_redir == 2)
			continue ;
		if (split_pipe(data, cmd, i))
			return (true);
		split_esc(data, cmd, i);
		split_qflags(data, cmd, i);
		if (split_col(data, cmd, i))
			return (true);
		split_def(data, cmd, i);
		data->flags->f_esc = false;
		if (!split_iter(cmd, i))
			break ;
	}
	return (false);
}

void	split_reset_flags(t_data *data)
{
	data->flags->f_dquote = false;
	data->flags->f_squote = false;
	data->flags->f_esc = false;
}

bool	split_quotes(t_data *data, char *cmd, int *i)
{
	int		start_args;

	start_args = 0;
	split_reset_flags(data);
	if (alloc_mem_array(data, cmd))
	{
		data->parser.arg_start = (*i);
		start_args = (*i);
		data->parser.array_index = 0;
		if (split_parse(data, cmd, i, start_args))
			return (true);
		if (cmd[*i] || data->flags->heredoc || !data->flags->heredoc)
		{
			parse_string(data, cmd, (*i), true);
			data->argv[data->parser.array_index] = NULL;
		}
		else
			data->argv[data->parser.array_index] = NULL;
		return (true);
	}
	return (false);
}
