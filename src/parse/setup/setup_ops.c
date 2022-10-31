/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:40:07 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 17:04:33 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	andor(t_data *data, char *cmd, int *i, int start_args)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc)
	{
		if (!ft_strncmp(cmd + (*i), "&&", 2))
			return (setup_argv_parse_and(data, i, start_args));
		if (!ft_strncmp(cmd + (*i), "||", 2))
			return (setup_argv_parse_or(data, i, start_args));
	}
	return (false);
}

bool	escape(t_data *data, char *cmd, int *i)
{
	if (cmd[*i] == '\\')
	{
		data->flags->f_esc = true;
		if (cmd[(*i)] && cmd[(*i) + 1])
			(*i)++;
		return (true);
	}
	return (false);
}

bool	pipe_(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc
		&& cmd[*i] == '|')
	{
		data->counter_pipes++;
		return (setup_argv_parse_pipe(data, i));
	}
	return (false);
}

bool	col(t_data *data, char *cmd, int *i)
{
	if ((!data->flags->f_dquote && !data->flags->f_esc)
		&& (cmd[*i] == ';' || (cmd[*i] == '&' && cmd[(*i) + 1])))
		return (semicolon(data, cmd, i));
	return (false);
}

bool	semicolon(t_data *data, char *cmd, int *i)
{
	while (waitpid(-1, &data->exit_code, 0) != -1)
	{
		if (WIFEXITED(data->exit_code))
			data->exit_status = WEXITSTATUS(data->exit_code);
	}
	setup_argv_write_arg(data, cmd, (*i), false);
	(*i) += 1;
	return (true);
}
