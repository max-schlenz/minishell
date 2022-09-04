/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:46:30 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/04 15:57:12 by tdehne           ###   ########.fr       */
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
	int		i;
	int		len;

	i = 0;
	if (data->args[1] && ft_isalnum(data->args[1][0]))
	{
		len = strlen_var(data->args[1]);
		while (data->envp[i])
		{
			if (ft_strncmp(data->envp[i], data->args[1], len) == 0)
			{
				free(data->envp[i]);
				data->envp[i] = ft_strdup(data->args[1]);
			}
			i++;
		}
		if (!data->envp[i])
			realloc_envp(data, 1);
		return (true);
	}
	printf("error\n");
	return (false);
}