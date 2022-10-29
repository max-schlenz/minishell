/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/29 16:32:06 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	setup_def(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& cmd[*i] == ' ' && cmd[(*i) + 1] && cmd[(*i) + 1] != ' ')
		setup_argv_parse_arg(data, cmd, (*i), false);
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
	int	flag_redir;

	flag_redir = 0;
	while (cmd[*i])
	{
		if (setup_andor(data, cmd, i, start_args))
			return (true);
		flag_redir = setup_redir(data, cmd, i);
		if (flag_redir == 1)
			return (true);
		if (flag_redir == 2)
			continue ;
		if (setup_pipe(data, cmd, i))
			return (true);
		setup_subshell(data, cmd, i);
		setup_esc(data, cmd, i);
		setup_qflags(data, cmd, i);
		if (setup_col(data, cmd, i))
			return (true);
		setup_def(data, cmd, i);
		data->flags->f_esc = false;
		if (!setup_iter(cmd, i))
			break ;
	}
	return (false);
}

void	setup_reset_flags(t_data *data)
{
	data->flags->f_dquote = false;
	data->flags->f_squote = false;
	data->flags->f_esc = false;
}

bool	setup_argv(t_data *data, char *cmd, int *i, bool alloc)
{
	int		start_args;

	start_args = 0;
	setup_reset_flags(data);
	if (alloc)
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
