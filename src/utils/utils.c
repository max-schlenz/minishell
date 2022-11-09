/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/09 17:42:34 by mschlenz         ###   ########.fr       */
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

static void	builtin_exec(t_data *data)
{
	if (!data->pid && data->flags->pipe)
		exec_redirs_pipes(data);
	if (!ft_strncmp(data->argv[0], "exit", 5))
		builtin_exit(data);
	else if (!ft_strncmp(data->argv[0], "cd", 3))
		builtin_cd(data);
	else if (!ft_strncmp(data->argv[0], "export", 7))
		builtin_export(data, NULL);
	else if (!ft_strncmp(data->argv[0], "unset", 6))
		builtin_unset(data);
	else if (!ft_strncmp(data->argv[0], "echo", 5))
		builtin_echo(data);
	else if (!ft_strncmp(data->argv[0], "env", 4))
		builtin_env(data);
	else if (!ft_strncmp(data->argv[0], "pwd", 4))
		builtin_pwd(data);
	else if (!ft_strncmp(data->argv[0], "color", 6))
		builtin_color(data, NULL);
	else if (!ft_strncmp(data->argv[0], "history", 8))
		builtin_history(data);
	else if (!ft_strncmp(data->argv[0], "where", 8))
		builtin_where(data);
}

bool	builtin(t_data *data)
{
	if (!exec_is_builtin(data))
		return (false);
	data->pid = 1;
	if (data->argv[0])
	{
		if (data->flags->pipe)
			exec_builtin_fork(data, false);
		if (data->flags->redir_out || data->flags->redir_append)
			exec_redirs_pipes(data);
		if ((data->flags->pipe && !data->pid) \
		|| (!data->flags->pipe && data->pid))
			builtin_exec(data);
		if (data->flags->pipe)
		{
			if (!data->pid)
				exit(0);
			exec_builtin_fork(data, true);
		}
	}
	return (true);
}

char	*realloc_arg(t_data *data, char *cmd, int index)
{
	char	*ret;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len);
	if (!ret)
		cleanup(data, E_MEM);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (i == index)
			i++;
		ret[j++] = cmd[i];
		if (cmd[i] && cmd[i + 1])
			i++;
		else
			break ;
	}
	ret[j] = '\0';
	free(cmd);
	data->cmd = NULL;
	return (ret);
}
