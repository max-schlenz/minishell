/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:23:56 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/13 12:26:18 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	set_filenames(t_data *data, int *i, char *cmd, int flag)
{
	size_t	start;

	start = *i;
	if (start >= ft_strlen(cmd))
	{
		write(2, "Syntax error\n", 14);
		data->exit_status = 2;
		return (false);
	}
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (!flag)
		data->file_name = ft_substr(cmd, start, *i - start);
	else if (flag == 1)
		data->file_name2 = ft_substr(cmd, start, *i - start);
	else if (flag == 2)
		data->file_name_append = ft_substr(cmd, start, *i - start);
	if (!cmd[*i])
		return (false);
	return (true);
}