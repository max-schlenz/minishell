/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:28:57 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/26 16:29:24 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	syntax_err(t_data *data, char *cmd)
{
	int		i;
	int		j;
	char	*ops;

	i = 0;
	j = 0;
	ops = "|&><";
	while (cmd[i] && cmd[i + 1] && cmd[i + 2])
	{
		while (ops[j])
		{
			if (cmd[i] == ops[j])
			{
				if (!syntax_err_consecutive(data, cmd, &i, &j))
					return (false);
			}
			j++;
		}
		j = 0;
		i++;
	}
	return (true);
}
