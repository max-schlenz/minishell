/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:46:30 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/07 14:12:18 by mschlenz         ###   ########.fr       */
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

bool	builtin_unset(t_data *data)
{
	int i;

	i = 0;
	if (data->argv[1])
	{
		while (data->envp[i])
		{
			if (!ft_strncmp(data->envp[i], data->argv[1], ft_strlen(data->argv[1])))
				break;
			i++;
		}
		while (i < data->counter_env)
		{
			data->envp[i] = data->envp[i + 1];
			i++;
		}
		free(data->envp[i]);
		data->envp[i] = NULL;
		sort_array(data);
		return (true);
	}
	return (true);
}

bool	builtin_color(t_data *data)
{
	int		i;
	char	*code;

	i = 0;
	if (!data->argv[1])
	{
		printf("change the color of your prompt!\n\n \
		d - \033[34;1mdef\033[36;1mault\033[0m\n \
		w - \033[0;34mwhite\033[0m\n \
		r - \033[0;31mred\033[0m\n \
		g - \033[0;32mgreen\033[0m\n \
		y - \033[0;33myellow\033[0m\n \
		b - \033[0;34mblue\033[0m\n \
		m - \033[0;35mmagenta\033[0m\n \
		c - \033[0;36mcyan\033[0m\n\n");
		
		data->argv[1] = readline("color code: ");
	}
	if (data->argv[1])
	{
		if (!ft_strncmp(data->argv[1], "white", 6) || !ft_strncmp(data->argv[1], "w", 2))
			data->prompt = "\033[34;1mmini\033[36;1mshell\033[0m >$ ";
		else if (!ft_strncmp(data->argv[1], "default", 8) || !ft_strncmp(data->argv[1], "d", 2))
			data->prompt = "minishell >$ ";
		else if (!ft_strncmp(data->argv[1], "red", 4) || !ft_strncmp(data->argv[1], "r", 2))
			data->prompt = "\033[0;31mminishell\033[0m >$ ";
		else if (!ft_strncmp(data->argv[1], "green", 6) || !ft_strncmp(data->argv[1], "g", 2))
			data->prompt = "\033[0;32mminishell\033[0m >$ ";
		else if (!ft_strncmp(data->argv[1], "yellow", 7) || !ft_strncmp(data->argv[1], "y", 2))
			data->prompt = "\033[0;33mminishell\033[0m >$ ";
		else if (!ft_strncmp(data->argv[1], "blue", 5) || !ft_strncmp(data->argv[1], "b", 2))
			data->prompt = "\033[0;34mminishell\033[0m >$ ";
		else if (!ft_strncmp(data->argv[1], "magenta", 8) || !ft_strncmp(data->argv[1], "m", 2))
			data->prompt = "\033[0;35mminishell\033[0m >$ ";
		else if (!ft_strncmp(data->argv[1], "cyan", 5) || !ft_strncmp(data->argv[1], "c", 2))
			data->prompt = "\033[0;36mminishell\033[0m >$ ";
	}
	return (true);
}
