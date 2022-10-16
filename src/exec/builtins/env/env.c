/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:29 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/16 10:33:29 by mschlenz         ###   ########.fr       */
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
		err = strjoin_nl
			("Error: env: parameters not supported: ", data->argv[1]);
		write(2, err, ft_strlen(err));
		free (err);
	}
	return (true);
}
