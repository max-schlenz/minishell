/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 11:19:40 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/28 12:44:29 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	export_subshell_init(t_data *data)
{
	data->subshell.buf = NULL;
	data->subshell.vcontent = NULL;
	data->subshell.vname = NULL;
	data->subshell.vname_eq = NULL;
	data->subshell.var_tmp = NULL;
	data->subshell.var = NULL;
}

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
	int	err;
	int	exit;

	err = 0;
	exit = 1;
	if (setv && setv[0] == '-')
	{
		exit = 2;
		err = 1;
	}
	else if (setv && !export_check_str(setv))
		err = 2;
	else if (setv && !isidentifier(setv[0]))
		err = 3;
	if (err)
		builtin_error(data, err, setv, exit);
	else
		export_setv(data, setv);
	if (data->export.free_set)
		free_null(1, setv);
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}

void	export_output(int len, char *name, char *val)
{
	if (!len)
		printf("declare -x %s\n", name);
	else
		printf("declare -x %s=\"%s\"\n", name, val);
}
