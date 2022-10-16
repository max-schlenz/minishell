/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 11:19:40 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/16 11:04:27 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	export_check_str(char *str)
{
	int		i;

	i = 0;
	while (str[i]
		&& ((str[i] >= 65 && str[i] <= 90)
			|| (str[i] >= 97 && str[i] <= 122)
			|| (str[i] >= 48 && str[i] <= 57)
			|| (str[i] == '_' || str[i] == '+')))
		i++;
	if (str[i] && str[i] != '=')
		return (false);
	i -= 2;
	if (i > 1)
	{
		while (str[i] && str[i] != '+')
		{
			i--;
			if (i < 0)
				break ;
		}
		if (i >= 0)
			return (false);
	}
	return (true);
}

bool	export_var(t_data *data, char *setv)
{
	if (setv && setv[0] == '-')
		return (export_err_op(data, setv));
	if (setv && !export_check_str(setv))
		return (export_err_con(data, setv));
	if (setv && isidentifier(setv[0]))
		return (export_setv(data, setv));
	return (export_err_inv(data, setv));
}

void	export_output(int len, char *name, char *val)
{
	if (!len)
		printf("declare -x %s\n", name);
	else
		printf("declare -x %s=\"%s\"\n", name, val);
}
