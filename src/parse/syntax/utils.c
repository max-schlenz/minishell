/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 22:03:49 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 12:33:43 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_syntax_iter(char *cmd, int *i)
{
	if (cmd[(*i)] == '\'' || cmd[(*i)++] == '\"')
	{
		(*i)++;
		return (true);
	}
	(*i)++;
	return (false);
}

bool	*err_msg(char *err_c)
{
	char	*err;

	err = strjoin_nl("Syntax error: ", err_c);
	write (2, err, ft_strlen(err));
	free (err);
	free (err_c);
	return (false);
}

char	*err_type(t_data *data, char c, int exit_status, int flag)
{
	char	err_c[3];

	if (!flag)
	{
		err_c[0] = c;
		err_c[1] = '\0';
	}
	else
	{
		err_c[0] = c;
		err_c[1] = c;
		err_c[2] = '\0';
	}
	data->exit_status = exit_status;
	return (ft_strdup(err_c));
}

bool	syntax_err_msg(t_data *data, char *ops, int i)
{
	char	*err;
	char	err_c[2];

	data->exit_status = 2;
	err_c[0] = ops[i];
	err_c[1] = '\0';
	err = strjoin_nl("Syntax error: ", err_c);
	write (2, err, ft_strlen(err));
	free (err);
	return (false);
}

bool	check_syntax_helper(char *cmd, char *ops, int i)
{
	if (cmd[i] && cmd[i + 1] && cmd[i + 2] && cmd[i] == *ops
		&& cmd[i + 1] == *ops && cmd[i + 2] == *ops)
		return (true);
	return (false);
}
