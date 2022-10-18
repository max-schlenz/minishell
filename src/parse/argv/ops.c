/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:40:07 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 13:01:44 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	split_andor(t_data *data, char *cmd, int *i, int start_args)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc)
	{
		if (!ft_strncmp(cmd + (*i), "&&", 2))
			return (parse_and(data, cmd, i, start_args));
		if (!ft_strncmp(cmd + (*i), "||", 2))
			return (parse_or(data, cmd, i, start_args));
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
		return (parse_pipes(data, i));
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
	parse_string(data, cmd, (*i), false);
	(*i) += 1;
	return (true);
}
