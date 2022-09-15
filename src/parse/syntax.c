/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/15 11:05:48 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_syntax(t_data *data)
{
	int		i;
	char	*ops_supported;
	char	*ops_unsupported;
	bool	quote;

	ops_supported = ft_strdup("|&><");
	ops_unsupported = ft_strdup("{");
	while (*ops_supported)
	{
		i = 0;
		while (data->cmd[i] && data->cmd[i + 1] && data->cmd[i + 2])
		{
			if (data->cmd[i] == '\'' || data->cmd[i] == '\"')
			{
				i++;
				while (data->cmd[i] && data->cmd[i + 1] && data->cmd[i + 2])
				{
					if (data->cmd[i] == '\'' || data->cmd[i++] == '\"')
						break ;
					i++;
				}
			}
			if	(data->cmd[i]
			&&	data->cmd[i + 1]
			&&	data->cmd[i + 2]
			&&	data->cmd[i] == *ops_supported
			&&	data->cmd[i + 1] == *ops_supported
			&&	data->cmd[i + 2] == *ops_supported)
			{
				printf("Syntax error: '%c' [%d]\n", *ops_supported, i + 3);
				return (false);
			}
			i++;
		}
		ops_supported++;
	}
	while (*ops_unsupported)
	{
		i = 0;
		while (data->cmd[i])
		{
			if (data->cmd[i] == '\'' || data->cmd[i] == '\"')
			{
				i++;
				while (data->cmd[i])
				{
					if (data->cmd[i] == '\'' || data->cmd[i] == '\"')
						break ;
					i++;
				}
			}
			if	(data->cmd[i] == *ops_unsupported)
			{
				printf("Syntax error: '%c' [%d] unsupported operation\n", *ops_unsupported, i + 3);
				return (false);
			}
			i++;
		}
		ops_unsupported++;
	}
	return (true);
}