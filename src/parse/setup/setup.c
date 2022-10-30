/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 14:00:39 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	setup_def(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& cmd[*i] == ' ' && cmd[(*i) + 1] && cmd[(*i) + 1] != ' ')
		{
			printf("in setip def %s\n", cmd + *i);
			setup_argv_parse_arg(data, cmd, (*i), false);
		}
}

static bool	setup_iter(char *cmd, int *i)
{
	if (cmd[(*i)])
	{
		(*i)++;
		return (true);
	}
	else
		return (false);
}

static bool	setup_argv_parse(t_data *data, char *cmd, int *i, int start_args)
{
	while (cmd[*i])
	{
		if (setup_andor(data, cmd, i, start_args))
			return (true);
		if (setup_redir(data, cmd, i))
		{
			return (true);
		}
		if (setup_pipe(data, cmd, i))
			return (true);
		setup_subshell(data, cmd, i);
		setup_esc(data, cmd, i);
		setup_qflags(data, cmd, i);
		if (setup_col(data, cmd, i))
			return (true);
		setup_def(data, cmd, i);
		data->flags->f_esc = false;
		if (cmd[*i] && cmd[*i] != '<' && cmd[*i] != '>')
		{
			(*i)++;
		}
	}
	return (false);
}

void	setup_reset_flags(t_data *data)
{
	data->flags->f_dquote = false;
	data->flags->f_squote = false;
	data->flags->f_esc = false;
}

bool	setup_argv(t_data *data, char *cmd, int *i)
{
	int		start_args;

	start_args = 0;
	setup_reset_flags(data);
	setup_alloc_argv(data, cmd);
	// ft_putendl_fd("setup argv", 2);
	// if (setup_alloc_argv(data, cmd))
	// {
		data->parser.arg_start = (*i);
		start_args = (*i);
		data->parser.array_index = 0;
		if (setup_argv_parse(data, cmd, i, start_args))
			return (true);
		if (cmd[*i] || data->flags->heredoc || !data->flags->heredoc)
		{
			setup_argv_parse_arg(data, cmd, (*i), true);
			data->argv[data->parser.array_index] = NULL;
		}
		else
			data->argv[data->parser.array_index] = NULL;
		return (true);
	// }
	return (false);
}
