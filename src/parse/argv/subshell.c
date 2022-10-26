/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:32:50 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/26 16:33:12 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	split_subshell(t_data *data, char *cmd, int *i)
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
