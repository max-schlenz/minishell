/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/08 18:34:25 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	alloc_argv(t_data *data, int mem)
{
	if (!data->flags->f_dquote && !data->flags->f_squote)
	{
		data->argv = ft_calloc(mem + 2, (sizeof(char *)));
		if (!data->argv)
			cleanup(data, E_MEM);
		return (true);
	}
	return (false);
}

//	allocates memory for argv and checks for unclosed quotes
bool	setup_alloc_argv(t_data *data, char *cmd)
{
	int		mem;
	int		i;

	mem = 1;
	i = 0;
	setup_reset_flags(data);
	while (cmd[i])
	{
		escape(data, cmd, &i);
		quote_flags(data, cmd, &i);
		if (!data->flags->f_dquote && !data->flags->f_squote
			&& cmd[i] && cmd[i] == ' ' && cmd[i + 1] && cmd[i + 1] != ' '
			&& cmd[i + 1] != '|' && cmd[i + 1] != '&')
				mem++;
		i++;
		data->flags->f_esc = false;
	}
	if (alloc_argv(data, mem))
		return (true);
	else
		printf(E_NC_QUOTE);
	return (false);
}

//	write command to argv
void	setup_def(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& cmd[(*i)]
		&& cmd[(*i) + 1]
		&& cmd[(*i)] == ' '
		&& cmd[(*i) + 1] != ' ')
		setup_argv_write_arg(data, cmd, (*i), false);
}

//	parse cmd, set parameters for execution and write argv
static bool	setup_argv_parse(t_data *data, char *cmd, int *i, int start_args)
{
	while (cmd[*i])
	{
		escape(data, cmd, i);
		if (andor(data, cmd, i, start_args))
			return (true);
		if (redir(data, cmd, i))
			return (true);
		if (pipe_(data, cmd, i))
			return (true);
		quote_flags(data, cmd, i);
		if (col(data, cmd, i))
			return (true);
		setup_def(data, cmd, i);
		if (cmd[*i] && ((cmd[*i] != '<' && cmd[*i] != '>') || data->flags->f_esc
				|| data->flags->f_dquote || data->flags->f_squote))
			(*i)++;
		data->flags->f_esc = false;
	}
	return (false);
}

//	setup command table for execution
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
