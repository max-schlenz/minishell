/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 11:22:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 15:01:58 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	exit_code_thing(int exit_status)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < exit_status)
	{
		if (ret > 255)
			ret = 0;
		ret++;
		i++;
	}
	i = 0;
	while (i > exit_status)
	{
		if (ret < 0)
			ret = 255;
		ret--;
		i--;
	}
	return (ret);
}

static bool	strnum(char *str)
{
	size_t	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i] && (str[i] >= 48 && str[i] <= 57))
		i++;
	if (i < ft_strlen(str))
		return (false);
	return (true);
}

static bool	exit_err(t_data *data)
{
	char	*err;

	err = strjoin_nl(E_EXIT_REQNO, data->argv[1]);
	write (2, err, ft_strlen(err));
	free (err);
	if (data->flags->macos)
		data->exit_status = 255;
	else
		data->exit_status = 2;
	return (true);
}

bool	builtin_exit(t_data *data)
{
	write(2, "exit\n", 6);
	if (data->argc > 1)
	{
		write(2, E_TM_ARG, 27);
		data->exit_status = 1;
	}
	else if (data->argv[1] && strnum(data->argv[1]))
	{
		data->exit_status = ms_atoll(data, data->argv[1]);
		if (data->flags->exit_code_of)
			exit_err(data);
		if (data->exit_status > 255)
			data->exit_status = exit_code_thing(data->exit_status);
	}
	if (data->argv[1] && !strnum(data->argv[1]))
		exit_err(data);
	free_array(data->argv);
	cleanup(data, 0);
	return (true);
}

long long	ms_atoll(t_data *data, const char *str)
{
	data->exit.i = 0;
	data->exit.r = 0;
	data->exit.neg = 1;
	if (str[data->exit.i] == '-' || str[data->exit.i] == '+')
	{
		if (str[data->exit.i] == '-')
			data->exit.neg = -1;
	data->exit.i++;
	}
	while (ft_isdigit(str[data->exit.i]))
	{
		if ((((!data->exit.neg && (data->exit.r > data->exit.max))
					|| (data->exit.r == data->exit.max
						&& str[data->exit.i] > '7')))
			|| ((data->exit.neg && (data->exit.r > data->exit.max))
				|| (data->exit.r == data->exit.max && str[data->exit.i] > '8')))
		{
			data->flags->exit_code_of = true;
			return (0);
		}
		data->exit.r = data->exit.r * 10 + str[data->exit.i] - '0';
		data->exit.i++;
	}
	return (data->exit.r * data->exit.neg);
}
