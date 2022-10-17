/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 19:33:48 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/17 21:18:57 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_error(t_data *data, int err, char *info, int exit)
{
	char	*msg;

	if (err == 1)
		msg = E_EXPORT_OPT;
	else if (err == 2)
		msg = E_EXPORT_CONT;
	else if (err == 3)
		msg = E_EXPORT_IDENT;
	else
		msg = "";
	write(2, msg, ft_strlen(msg));
	write(2, info, ft_strlen(info));
	write(2, "\n", 1);
	data->exit_status = exit;
}

bool	builtin_error(t_data *data, char *err)
{
	char	*msg;

	msg = E_BUILTIN_INVOPT;
	write(2, msg, 29);
	write(2, err, 2);
	write(2, "\n", 1);
	data->exit_status = 2;
	return (true);
}
