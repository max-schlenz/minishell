/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:23:56 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 16:45:21 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	setup_alloc_argv(t_data *data, char *cmd)
{
	int		mem;
	int		i;

	mem = 1;
	i = 0;
	setup_reset_flags(data);
	while (cmd[i])
	{
		setup_esc(data, cmd, &i);
		setup_qflags(data, cmd, &i);
		setup_subshell(data, cmd, &i);
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
		return (printf(E_NC_QUOTE), false);
}

void	setup_qflags(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_esc)
	{
		if (cmd[*i] == '\"' && !data->flags->f_squote)
			data->flags->f_dquote = !data->flags->f_dquote;
		if (cmd[*i] == '\'' && !data->flags->f_dquote)
			data->flags->f_squote = !data->flags->f_squote;
	}
}

bool	setup_all_filenames(t_data *data, int *i, char *cmd, int flag)
{
	size_t	start;

	start = *i;
	if (start > ft_strlen(cmd))
	{
		write(2, "Syntax error\n", 14);
		return (data->exit_status = 2, (*i) = ft_strlen(cmd), false);
	}
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (!flag)
	{
		data->file_name = ft_substr(cmd, start, *i - start);
		if (access((data->file_name), F_OK) == -1)
			return (builtin_error(data, 8, data->file_name, 1),
				(*i) = ft_strlen(cmd), false);
	}
	else if (flag == 1)
		data->file_name2 = ft_substr(cmd, start, *i - start);
	else if (flag == 2)
		data->file_name_append = ft_substr(cmd, start, *i - start);
	if (!cmd[*i])
		return (true);
	return (false);
}

void	remove_quotes_init(t_data *data)
{
	data->rmq.start = 0;
	data->rmq.end = 0;
	data->rmq.f_rmq = false;
}
