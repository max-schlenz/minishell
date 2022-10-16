/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:26 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/16 14:56:24 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	echo_n_flag(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	j = 0;
	k = 0;
	while (data->argv[i][j])
	{
		if (data->argv[i][j] == '-')
			k++;
		j++;
	}
	if (k == 1)
	{
		i++;
		return (true);
	}
	return (false);
}

static bool	echo_print(t_data *data, int i)
{
	size_t	j;

	j = 0;
	while (data->argv[i] && data->argv[i][j])
	{
		if (i <= data->argc
			&& (!ft_strncmp(data->argv[i], "\'\'", 2)
				|| !ft_strncmp(data->argv[i], "\"\"", 2)))
		{
			if (i == data->argc)
				return (false);
			else
				j += 2;
			if (j > ft_strlen(data->argv[i]) - 1)
				break ;
		}
		else if (data->argv[i][j] != '\\')
		{
			printf("%c", data->argv[i][j]);
			data->echo.f_fc = true;
		}
		j++;
	}
	return (true);
}

bool	builtin_echo(t_data *data)
{
	bool	echo_n;
	int		i;

	i = 1;
	data->echo.f_fc = false;
	echo_n = false;
	if (data->argv[i] && !ft_strncmp(data->argv[i], "-n", 2))
		echo_n = echo_n_flag(data);
	while (data->argv[i])
	{
		while (echo_n && data->argv[i] && !ft_strncmp(data->argv[i], "-n", 2))
			i++;
		if (!echo_print(data, i))
			break ;
		if (data->argv[i])
			i++;
		if (data->echo.f_fc && data->argv[i]
			&& ft_strlen(data->argv[i]) > 0 && (!(echo_n && i < 3)))
			printf(" ");
	}
	if (!echo_n)
		printf("\n");
	data->exit_status = 0;
	return (true);
}
