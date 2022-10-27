/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:57:17 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 22:31:24 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pre_parse_check_ops(t_data *data, char **cmd, char op, int i)
{
	if ((*cmd) && (*cmd)[i] && (*cmd)[i + 1] && (*cmd)[i] != ' '
		&& (*cmd)[i] != op && (*cmd)[i + 1] == op)
		*cmd = insert_space(data, (*cmd), i);
	else if (*cmd && (*cmd)[i] && (*cmd)[i + 1] && (*cmd)[i + 1] != ' '
		&& (*cmd)[i + 1] != op && (*cmd)[i] == op)
		*cmd = insert_space(data, (*cmd), i);
}
