/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:46:30 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/07 11:26:30 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	builtin_cd(t_data *data)
{
	char *path;
	
	if (data->argv[1])
	{
		path = ft_strtrim(data->argv[1], " ");
		chdir(path);
		free (path);
		return (true);
	}
	return (false);
}

bool	builtin_echo(t_data *data)
{
	int 	i = 1;
	char	*cmd_trim;
	bool	echo_n = false;

	if (data->argv[i] && !ft_strncmp(data->argv[i], "-n", 2))
		echo_n = true;
	while (data->argv[i])
	{
		if (data->argv[i][0] != '-')
		{
			cmd_trim = ft_strtrim(data->argv[i], " ");
			ft_printf("%s ", cmd_trim);
			free(cmd_trim);
		}
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
	if (data->argv[1] && ft_isalnum(data->argv[1][0]))
	{
		len = strlen_var(data->argv[1]);
		while (data->envp[i])
		{
			if (ft_strncmp(data->envp[i], data->argv[1], len) == 0)
			{
				free(data->envp[i]);
				data->envp[i] = ft_strdup(data->argv[1]);
			}
			i++;
		}
		if (!data->envp[i])
			realloc_envp(data, 1);
		sort_array(data);
		parse_path(data);
		return (true);
	}
	i = 0;
	while (data->envp[i])
		printf("declare -x %s\n", data->envp[i++]);
	return (true);
}

bool	builtin_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
		printf("%s\n", data->envp[i++]);
	return (true);
}

bool	builtin_pwd(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PWD", 3))
		i++;
	printf("%s\n", data->envp[i] + 4);
	return (true);
}