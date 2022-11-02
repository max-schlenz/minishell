/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:23:56 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/02 14:34:23 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	setup_reset_flags(t_data *data)
{
	data->flags->f_dquote = false;
	data->flags->f_squote = false;
	data->flags->f_esc = false;
}

void	setup_def(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& cmd[(*i)]
		&& cmd[(*i) + 1]
		&& cmd[(*i)] == ' '
		&& cmd[(*i) + 1] != ' ')
		setup_argv_write_arg(data, cmd, (*i), false);
}

bool	setup_alloc_argv(t_data *data, char *cmd)
{
	int		mem;
	int		i;

	mem = 1;
	i = 0;
	setup_reset_flags(data);
	while (cmd[i])
	{
		escape(data, cmd, &i);
		quote_flags(data, cmd, &i);
		subshell(data, cmd, &i);
		if (!data->flags->f_dquote && !data->flags->f_squote
			&& cmd[i] && cmd[i] == ' ' && cmd[i + 1] && cmd[i + 1] != ' ' 
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

void	quote_flags(t_data *data, char *cmd, int *i)
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
	if (!cmd[*i] || !ft_strncmp(cmd + (*i) + 1, "&&", 2) || !ft_strncmp(cmd + (*i) + 1, "||", 2))
		return (true);
	return (false);
}

void	remove_quotes_init(t_data *data)
{
	data->rmq.start = 0;
	data->rmq.end = 0;
	data->rmq.f_rmq = false;
}
