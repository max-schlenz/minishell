/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:46:30 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/04 12:47:21 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	builtin_cd(t_data *data)
{
	char *path;
	
	if (data->args[1])
	{
		path = ft_strtrim(data->args[1], " ");
		chdir(path);
		free (path);
		return (true);
	}
	return (false);
}

bool	builtin_echo(t_data *data)
{
	int 	i = 1;
	bool	echo_n = false;

	if (data->args[i] && !ft_strncmp(data->args[i], "-n", 2))
		echo_n = true;
	while (data->args[i])
	{
		if (data->args[i][0] != '-')
			ft_printf("%s", data->args[i]);
		i++;
	}
	if (!echo_n)
		ft_printf("\n");
	return (true);
}

bool	builtin_export(t_data *data)
{
	int		len_arg;
	char	*var;
	char	*value;
	t_env	*node;
	
	if (data->args[1])
	{
		len_arg = ft_strlen(data->args[1]);
		value = ft_strchr(data->args[1], '=');
		value++;
		var = ft_substr(data->args[1], 0, len_arg - ft_strlen(value) - 1);
		node = ft_mslstnew(data, var, value);
		ft_mslstadd_front(&data->env, node);
		data->counter_envv++;
		return (true);
	}
	return (false);
}