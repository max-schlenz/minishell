/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 23:06:53 by mschlenz         ###   ########.fr       */
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

int	strdiff(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
	i++;
	}
	if ((!s1[i] && s2[i]) || (!s2[i] && s1[i]))
		return (1);
	return (0);
}

void	history(t_data *data)
{
	if (strdiff(data->cmd, data->last_cmd))
	{
		add_history(data->cmd);
		write(data->mscfg, "HISTORY=", 8);
		write(data->mscfg, data->cmd, ft_strlen(data->cmd));
		write(data->mscfg, "\n", 1);
	}
	if (data->last_cmd)
		free (data->last_cmd);
	data->last_cmd = ft_strdup(data->cmd);
}

long long	ms_atoll(t_data *data, const char *str)
{
	long long	i;
	long long	r;
	long long	is_neg;

	i = 0;
	r = 0;
	is_neg = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
			i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		is_neg = -1;
	i++;
	}
	while (ft_isdigit(str[i]))
	{
		if (((!is_neg && ((r > 922337203685477580)
		|| (r == 922337203685477580 && str[i] > '7'))))
		|| (is_neg && ((r > 922337203685477580)
		|| (r == 922337203685477580 && str[i] > '8'))))
		{
			data->flags->exit_code_of = true;
			return (0);
		}
		r = r * 10 + str[i] - '0';
		i++;
	}
	return (r * is_neg);
}

char	*strjoin_nl(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (0);
	str = malloc((ft_strlen(s1)) + (ft_strlen(s2)) + 2);
	if (!str)
		return (0);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\n';
	str[i + 1] = '\0';
	return (str);
}

void	builtin_fork(t_data *data, bool flag)
{
	if (!flag && data->flags->pipe)
		data->pid = fork();
	else if (flag && data->flags->pipe)
	{
		waitpid(data->pid, &data->exit_code, 0);
		exec_close_pipes(data);
		exec_set_flags(data);
		data->exit_status = 0;
	}
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
