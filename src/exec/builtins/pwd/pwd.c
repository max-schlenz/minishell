/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:34 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 15:07:10 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	builtin_pwd(t_data *data)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	free (pwd);
	data->exit_status = 0;
	return (true);
}
