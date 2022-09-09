/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:46:30 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/09 11:59:40 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	builtin_cd(t_data *data)
{
	char	*path;
	char	*path_tmp;
	char	*path_tmp2;
	char	*new_pwd_tmp;
	int		i;
	int		j;
	
	i = 0;
	new_pwd_tmp = NULL;
	while (data->envp[i] && strcmp_alnum(data->envp[i], "PWD"))
		i++;
	if (data->argv[1] && !ft_strncmp(data->argv[1], ".", 2))
		return (true);
	if (data->argv[1])
	{
		path_tmp2 = ft_strtrim(data->argv[1], " ");
		if (data->argv[1][0] != '/')
		{
			path_tmp = ft_substr(data->envp[i], 3, ft_strlen(data->envp[i] - 3));
			path = ft_strjoin(path_tmp2, path_tmp);
		}
		else
		{
			new_pwd_tmp = ft_strjoin("PWD=", path_tmp2);
			path = path_tmp2;
		}
		if (!access(path, F_OK))
		{
			chdir(path);
			free (path);
			if (ft_strncmp(data->argv[1], "..", 3) && (!new_pwd_tmp))
			{
				if (data->envp[i][ft_strlen(data->envp[i]) - 1] != '/')
					path = ft_strjoin("/", data->argv[1]);
				else
					path = ft_strdup(data->argv[1]);
				new_pwd_tmp = ft_strjoin(data->envp[i], path);
			}
			else if (!new_pwd_tmp)
			{
				j = ft_strlen(data->envp[i]) - 1;
				while (data->envp[i][j] && data->envp[i][j] != '/')
					j--;
				new_pwd_tmp = ft_substr(data->envp[i], 0, j);
			}
		}
		else
			return (true);
	}
	else
	{
		chdir("/root");
		new_pwd_tmp = ft_strdup("PWD=/root");
	}
	free (data->envp[i]);
	data->envp[i] = ft_strdup(new_pwd_tmp);
	free (new_pwd_tmp);
	data->exit_status = 0;
	return (true);
}

bool	builtin_echo(t_data *data)
{
	int 	i = 1;
	char	*cmd_trim;
	bool	echo_n = false;
	bool	space = false;

	if (data->argv[i] && !ft_strncmp(data->argv[i], "-n", 2))
		echo_n = true;
	while (data->argv[i])
	{
		while (!ft_strncmp(data->argv[i], "-n", 3))
			i++;
		if (data->argv[i][0] != '-')
		{
			cmd_trim = ft_strtrim(data->argv[i], " ");
			printf("%s", cmd_trim);
			free(cmd_trim);
		}
		i++;
		if (data->argv[i] && (!(echo_n && i < 3)))
			printf(" ");
	}
	if (!echo_n)
		printf("\n");
	data->exit_status = 0;
	return (true);
}

bool	builtin_export(t_data *data)
{
	int		len_arg;
	char	*var;
	char	*value;
	int		i;
	int		len;
	int		len_name;
	int		len_content;
	char	*str_name;
	char	*str_content;

	i = 0;
	if (data->argv[1] && ft_isalnum(data->argv[1][0]))
	{
		len = strlen_var(data->argv[1]);
		if (len > ft_strlen(data->argv[1]) - 1)
		{
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
		}
		return (true);
	}
	if (!data->argv[1])
	{
		i = 0;
		while (data->envp[i])
		{
			len_name = strlen_var(data->envp[i]);
			len_content = ft_strlen(data->envp[i]) - len_name;
			str_name = ft_substr(data->envp[i], 0, len_name);
			str_content = ft_substr(data->envp[i], len_name + 1, len_content);
			printf("declare -x %s=\"%s\"\n", str_name, str_content);
			free (str_name);
			free (str_content);
			i++;
		}
		data->exit_status = 0;
	}
	return (true);
}

bool	builtin_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
		printf("%s\n", data->envp[i++]);
	data->exit_status = 0;
	return (true);
}

bool	builtin_pwd(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PWD", 3))
		i++;
	printf("%s\n", data->envp[i] + 4);
	data->exit_status = 0;
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
		data->exit_status = 0;
		return (true);
	}
	data->exit_status = 0;
	return (true);
}

bool	builtin_color(t_data *data)
{
	char	*code;

	printf("change the color of your prompt!\n\n \
	d - \033[34;1mdef\033[36;1mault\033[0m\n \
	w - \033[0;34mwhite\033[0m\n \
	r - \033[0;31mred\033[0m\n \
	g - \033[0;32mgreen\033[0m\n \
	y - \033[0;33myellow\033[0m\n \
	b - \033[0;34mblue\033[0m\n \
	m - \033[0;35mmagenta\033[0m\n \
	c - \033[0;36mcyan\033[0m\n\n");
	code = readline("enter color code: ");
	if (code)
	{
		if (!ft_strncmp(data->argv[1], "white", 6) || !ft_strncmp(data->argv[1], "w", 2))
			data->prompt = "minishell > ";
		else if (!ft_strncmp(data->argv[1], "default", 8) || !ft_strncmp(data->argv[1], "d", 2))
			data->prompt = "\033[34;1mmini\033[36;1mshell\033[0m > ";
		else if (!ft_strncmp(data->argv[1], "red", 4) || !ft_strncmp(data->argv[1], "r", 2))
			data->prompt = "\033[0;31mminishell\033[0m > ";
		else if (!ft_strncmp(data->argv[1], "green", 6) || !ft_strncmp(data->argv[1], "g", 2))
			data->prompt = "\033[0;32mminishell\033[0m > ";
		else if (!ft_strncmp(data->argv[1], "yellow", 7) || !ft_strncmp(data->argv[1], "y", 2))
			data->prompt = "\033[0;33mminishell\033[0m > ";
		else if (!ft_strncmp(data->argv[1], "blue", 5) || !ft_strncmp(data->argv[1], "b", 2))
			data->prompt = "\033[0;34mminishell\033[0m > ";
		else if (!ft_strncmp(data->argv[1], "magenta", 8) || !ft_strncmp(data->argv[1], "m", 2))
			data->prompt = "\033[0;35mminishell\033[0m > ";
		else if (!ft_strncmp(data->argv[1], "cyan", 5) || !ft_strncmp(data->argv[1], "c", 2))
			data->prompt = "\033[0;36mminishell\033[0m > ";
	}
	data->exit_status = 0;
	return (true);
}
