/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 18:02:10 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


static bool	setup_argv_parse(t_data *data, char *cmd, int *i, int start_args)
{
	while (cmd[*i])
	{
		if (andor(data, cmd, i, start_args))
			return (true);
		if (redir(data, cmd, i))
			return (true);
		if (pipe_(data, cmd, i))
			return (true);
		subshell(data, cmd, i);
		escape(data, cmd, i);
		quote_flags(data, cmd, i);
		if (col(data, cmd, i))
			return (true);
		setup_def(data, cmd, i);
		data->flags->f_esc = false;
		if (cmd[*i] && cmd[*i] != '<' && cmd[*i] != '>')
			(*i)++;
	}
	return (false);
}

bool	setup_argv(t_data *data, char *cmd, int *i)
{
	int		start_args;

	start_args = 0;
	setup_reset_flags(data);
	if (setup_alloc_argv(data, cmd))
	{
		start_args = (*i);
		data->parser.arg_start = (*i);
		data->parser.array_index = 0;
		if (setup_argv_parse(data, cmd, i, start_args))
			return (true);
		if (cmd[*i] || data->flags->heredoc || !data->flags->heredoc)
			setup_argv_write_arg(data, cmd, (*i), true);
		data->argv[data->parser.array_index] = NULL;
		return (true);
	}
	return (false);
}
