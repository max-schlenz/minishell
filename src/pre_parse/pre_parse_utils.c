/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:57:17 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/13 09:56:15 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pre_parse_check_ops(t_data *data, char **cmd, char op, int i)
{
	if ((*cmd) && (*cmd)[i] && (*cmd)[i + 1])
	{
		if ((*cmd)[i] != ' ' && (*cmd)[i] != op && (*cmd)[i + 1] == op)
			(*cmd) = pre_parse_insert_space(data, (*cmd), i);
		else if ((*cmd)[i + 1] != ' ' && (*cmd)[i + 1] != op && (*cmd)[i] == op)
			(*cmd) = pre_parse_insert_space(data, (*cmd), i);
	}
}

char	*remove_quotes_spaces(t_data *data, char *cmd)
{
	cmd = pre_parse_skip_d(data, cmd, ' ');
	cmd = strrmstr(cmd, "\"\"");
	cmd = strrmstr(cmd, "\'\'");
	data->cmd = NULL;
	return (cmd);
}
