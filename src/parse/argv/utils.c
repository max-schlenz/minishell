/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:23:56 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/19 09:16:49 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	rm_quotes_init(t_data *data)
{
	data->rmq.start = 0;
	data->rmq.end = 0;
	data->rmq.f_rmq = false;
}

bool	alloc_mem_array(t_data *data, char *cmd)
{
	int		mem;
	int		i;

	mem = 1;
	i = 0;
	split_reset_flags(data);
	while (cmd[i])
	{
		split_esc(data, cmd, &i);
		split_qflags(data, cmd, &i);
		if (!data->flags->f_dquote && !data->flags->f_squote
			&& cmd[i] == ' ' && cmd[i + 1] && cmd[i + 1] != ' '
			&& cmd[i + 1] != '|' && cmd[i + 1] != '&')
			mem++;
		i++;
		data->flags->f_esc = false;
	}
	if (!data->flags->f_dquote && !data->flags->f_squote)
	{
		data->argv = ft_calloc(mem + 2, (sizeof(char *)));
		return (true);
	}
	else
		printf(E_NC_QUOTE);
	return (false);
}

void	split_qflags(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_esc)
	{
		if (cmd[*i] == '\"' && !data->flags->f_squote)
			data->flags->f_dquote = !data->flags->f_dquote;
		if (cmd[*i] == '\'' && !data->flags->f_dquote)
			data->flags->f_squote = !data->flags->f_squote;
	}
}

bool	set_filenames(t_data *data, int *i, char *cmd, int flag)
{
	size_t	start;

	start = *i;
	if (start >= ft_strlen(cmd))
	{
		(*i) = ft_strlen(cmd);
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
