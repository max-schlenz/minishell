/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:32:50 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 16:59:47 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	subshell(t_data *data, char *cmd, int *i)
{
	size_t	popen;

	popen = 0;
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& cmd[*i] == '$' && cmd[(*i) + 1] && cmd[(*i) + 1] == '(')
	{
		(*i)++;
		while (cmd[*i])
		{
			if (cmd[*i] == '(')
				popen++;
			if (cmd[*i] == ')')
				popen--;
			if (!popen)
				break ;
			(*i)++;
		}
	}
}
