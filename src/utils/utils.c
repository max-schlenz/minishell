/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/15 18:12:26 by mschlenz         ###   ########.fr       */
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
	if (data->argv[0])
	{
		if (!ft_strncmp(data->argv[0], "exit", 5))
			return (builtin_exit(data));
		else if (!ft_strncmp(data->argv[0], "cd", 3))
			return (builtin_cd(data));
		else if (!ft_strncmp(data->argv[0], "export", 7))
			return (builtin_export(data, NULL));
		else if (!ft_strncmp(data->argv[0], "unset", 6))
			return (builtin_unset(data));
		else if (!ft_strncmp(data->argv[0], "color", 6))
			return (builtin_color(data, NULL));
		else if (!ft_strncmp(data->argv[0], "history", 8))
			return (builtin_history(data));
		else if (!ft_strncmp(data->argv[0], "tdebug", 7))
			return (builtin_dbg(data));
	}
	return (false);
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
