/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/17 16:17:27 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	isnumeric(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	isidentifier(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == '_')
		return (1);
	return (0);
}

bool	builtin_environment(t_data *data)
{
	if (!is_builtin(data))
		return (false);
	data->pid = 1;
	if (data->argv[0])
	{
		if (data->flags->pipe)
		{
			builtin_fork(data, false);
		}
		if (data->flags->pipe && data->pid == 0 || !data->flags->pipe && data->pid)
		{
			if (!data->pid)
				redirs_pipes(data);
			if (!ft_strncmp(data->argv[0], "exit", 5))
				builtin_exit(data);
			else if (!ft_strncmp(data->argv[0], "cd", 3))
				builtin_cd(data);
			else if (!ft_strncmp(data->argv[0], "export", 7))
				builtin_export(data, NULL);
			else if (!ft_strncmp(data->argv[0], "unset", 6))
				builtin_unset(data);
			else if (!ft_strncmp(data->argv[0], "color", 6))
				builtin_color(data, NULL);
			else if (!ft_strncmp(data->argv[0], "history", 8))
				builtin_history(data);
			else if (!ft_strncmp(data->argv[0], "tdebug", 7))
				builtin_dbg(data);
		}
		if (data->flags->pipe)
		{
			if (data->pid == 0)
				exit(0);
			builtin_fork(data, true);
		}
	}
	return (true);
}

bool	builtin_print(t_data *data)
{
	if (!ft_strncmp(data->argv[0], "echo", 5))
		return (builtin_echo(data));
	else if (!ft_strncmp(data->argv[0], "env", 4))
		return (builtin_env(data));
	else if (!ft_strncmp(data->argv[0], "pwd", 4))
		return (builtin_pwd(data));
	return (false);
}

char	*strmv(char *new)
{
	char	*ret;

	ret = ft_strdup(new);
	free (new);
	new = NULL;
	return (ret);
}
