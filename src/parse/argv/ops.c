/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:40:07 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/23 14:39:10 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	split_andor(t_data *data, char *cmd, int *i, int start_args)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc)
	{
		if (!ft_strncmp(cmd + (*i), "&&", 2))
			return (parse_and(data, i, start_args));
		if (!ft_strncmp(cmd + (*i), "||", 2))
			return (parse_or(data, i, start_args));
	}
	return (false);
}

bool	split_esc(t_data *data, char *cmd, int *i)
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

bool	split_pipe(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc
		&& cmd[*i] == '|')
	{
		data->counter_pipes++;
		return (parse_pipes(data, i));
	}
	return (false);
}

bool	split_col(t_data *data, char *cmd, int *i)
{
	if ((!data->flags->f_dquote && !data->flags->f_esc)
		&& (cmd[*i] == ';' || (cmd[*i] == '&' && cmd[(*i) + 1])))
		return (split_semicolon(data, cmd, i));
	return (false);
}

bool	split_semicolon(t_data *data, char *cmd, int *i)
{
	while (waitpid(-1, &data->exit_code, 0) != -1)
	{
		if (WIFEXITED(data->exit_code))
			data->exit_status = WEXITSTATUS(data->exit_code);
	}
	parse_string(data, cmd, (*i), false);
	(*i) += 1;
	return (true);
}
