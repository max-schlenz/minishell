/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/18 15:10:32 by mschlenz         ###   ########.fr       */
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
		if (((!is_neg && (r > 922337203685477580)
		|| (r == 922337203685477580 && str[i] > '7')))
		|| (is_neg && (r > 922337203685477580)
		|| (r == 922337203685477580 && str[i] > '8')))
		{
			data->flags->exit_code_of = true;
			return (0);
		}
		r = r * 10 + str[i] - '0';
		i++;
	}
	return (r * is_neg);
}
