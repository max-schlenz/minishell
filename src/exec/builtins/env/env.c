/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:29 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/16 20:27:48 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	builtin_env(t_data *data)
{
	int		i;
	char	*err;

	i = 0;
	if (!data->argv[1])
	{
		while (data->envp[i])
		{
			if (ft_strchr(data->envp[i], '='))
				printf("%s\n", data->envp[i]);
			i++;
		}
		data->exit_status = 0;
	}
	else
	{
		write(2, "Error: env: parameters not supported: ", 39);
		write(2, data->argv[1], ft_strlen(data->argv[1]));
		write(2, "\n", 1);
	}
	return (true);
}
