/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 19:33:48 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 13:56:22 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	builtin_error(t_data *data, int err, char *info, int exit)
{
	char	*msg;

	if (err == 1)
		msg = E_EXPORT_INVOPT;
	else if (err == 2)
		msg = E_EXPORT_CONT;
	else if (err == 3)
		msg = E_EXPORT_IDENT;
	else if (err == 4)
		msg = E_ENV_INVOPT;
	else if (err == 5)
		msg = E_ENV_IDENT;
	else if (err == 6)
		msg = E_EXECVE_MSG;
	else if (err == 7)
		msg = E_CD_OLDPWD_MSG;
	else if (err == 8)
		msg = E_FILE_NOT_FOUND;
	else
		msg = "";
	write(2, msg, ft_strlen(msg));
	write(2, info, ft_strlen(info));
	write(2, "\n", 1);
	data->exit_status = exit;
}
