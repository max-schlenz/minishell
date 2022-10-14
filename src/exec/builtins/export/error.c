/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 11:03:01 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 11:05:55 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	export_err_con(t_data *data, char *setv)
{
	data->export.err = strjoin_nl
		("Error: export: not valid in this context: ", setv);
	write(2, data->export.err, ft_strlen(data->export.err));
	free (data->export.err);
	if (data->export.free_set)
	{
		free (setv);
		setv = NULL;
	}
	data->exit_status = 1;
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}

bool	export_err_inv(t_data *data, char *setv)
{
	data->exit_status = 1;
	data->export.err = strjoin_nl
		("Error: export: not a valid identifier: ", setv);
	write(2, data->export.err, ft_strlen(data->export.err));
	free (data->export.err);
	if (data->export.free_set)
	{
		free (setv);
		setv = NULL;
	}
	return (true);
}

bool	export_err_op(t_data *data, char *setv)
{
	data->export.err = strjoin_nl
		("Error: export: option not supported: ", setv);
	write(2, data->export.err, ft_strlen(data->export.err));
	free (data->export.err);
	data->exit_status = 2;
	if (data->export.free_set)
	{
		free (setv);
		setv = NULL;
	}
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}
