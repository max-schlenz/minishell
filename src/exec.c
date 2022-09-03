/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 15:36:53 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 13:53:38 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void parse_var(t_data *data, char *var, int i)
{
	char	*rest;
	int		j;
	int		len_var;
	
	int g = 0;

	var++;
	len_var = strlen_path(var);
	while (j < data->counter_envv)
	{
		if (!ft_strncmp(var, data->env->var, len_var))
			break ;
		data->env = data->env->next;
		j++;
	}
	if (j >= data->counter_envv)
		data->args[i] = ft_strdup("");
	else
		data->args[i] = ft_strdup(data->env->content);
	rest = ft_substr(var, len_var, ft_strlen(var) - len_var);
	data->args[i] = ft_strjoin(data->args[i], rest);
}

void	parse_args(t_data *data)
{
	int		i;
	char	*var;
	char	*tmp;

	i = 0;
	data->args = ft_split(data->cmd, ' ');
	while (data->args[i])
	{
		if (chk_escvar(data->args[i]))
		{
			var = chk_escvar(data->args[i]);
			if (var[0] == '$')
				parse_var(data, var, i);
			else if (var[0] == '\'')
				data->args[i] = ft_substr(var, 1, ft_strlen(var) - 1);
			else if (var[0] == '\"')
			{
				var = ft_substr(var, 1, ft_strlen(var) - 2);
				if (var[0] == '$')
					parse_var(data, var, i);
			}
		}
		data->args[i] = ft_strtrim(data->args[i], "\"");
		data->args[i] = ft_strtrim(data->args[i], "\'");
		data->args[i] = ft_strjoin(data->args[i], " ");
		i++;
	}
}

static char *get_path(t_data *data)
{
	int		i;
	char	*abs_path;

	i = 0;
	while (data->path[i])
	{
		abs_path = ft_strjoin(data->path[i], "/");
		abs_path = ft_strjoin(abs_path, data->cmd);
		if (!access(abs_path, F_OK))
			return (abs_path);
		i++;
	}
	return (NULL);
}

static bool	builtin_cd(t_data *data)
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


static bool	builtin_echo(t_data *data)
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


static bool	builtin_export(t_data *data)
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


bool	builtins(t_data *data)
{
	if (!ft_strncmp(data->cmd, "echo", 4))
		return (builtin_echo(data));
	else if (!ft_strncmp(data->cmd, "cd", 2))
		return (builtin_cd(data));
	else if (!ft_strncmp(data->cmd, "export", 6))
		return (builtin_export(data));
	if (!ft_strncmp(data->cmd, "exit", 4) || data->cmd[0] == '\0')
		cleanup(data, 0);
	return (false);
}

void	exec_program(t_data *data, char **envp)
{
	pid_t		pid;
	int			exit_code;
	char		*abs_path;
	
	abs_path = get_path(data);
	if (!abs_path)
		abs_path = ft_strdup(data->cmd);
	if (!access(abs_path, F_OK))
	{
		pid = fork();
		if (pid == 0)
			execve(abs_path, data->args, envp);
		waitpid(pid, NULL, 0);
		exit_code = WEXITSTATUS(pid);
	}
}