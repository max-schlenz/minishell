/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:29 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 17:32:43 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	builtin_env(t_data *data)
{
	int		i;

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
		builtin_error(data, 4, data->argv[1], 2);
	return (true);
}
