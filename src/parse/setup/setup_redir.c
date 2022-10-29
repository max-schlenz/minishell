/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:31:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/29 12:54:47 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	setup_redir_files(t_data *data, char *cmd, int *i)
{
	if (!data->flags->redir_out)
	{
		if (!setup_filenames(data, i, cmd, 0))
			return (true);
	}
	else
	{
		if (!setup_filenames(data, i, cmd, 1))
			return (true);
	}
	return (false);
}

// data->flags->redir_in = false;
static bool	setup_redir_out(t_data *data)
{
	int		tmp_fd;

	data->flags->redir_out = false;
	data->flags->redir_append = false;
	if (data->file_name2)
	{
		tmp_fd = exec_redirs_pipes_fopen(data, data->file_name2, 0);
		close(tmp_fd);
	}
	return (false);
}

static bool	setup_redir_util(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& !ft_strncmp(cmd + (*i), ">>", 2))
		return (setup_argv_is_redir_out(data, cmd, i));
	else if (cmd[*i] == '>')
		data->flags->redir_out = true;
	else if (cmd[*i] == '<')
		data->flags->redir_in = true;
	(*i) += 2;
	data->argv[data->parser.array_index] = NULL;
	if (setup_redir_files(data, cmd, i))
		return (true);
	(*i)++;
	if (cmd[*i] && cmd[*i] == '|')
	{
		if (setup_pipe(data, cmd, i))
			return (true);
	}
	if (cmd[*i] && cmd[*i] == '>')
	{
		setup_redir_out(data);
		return (false);
	}
	return (true);
}

int	setup_redir(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc
		&& (cmd[*i] == '>' || cmd[*i] == '<'))
	{
		if (setup_redir_util(data, cmd, i))
			return (1);
		else
			return (2);
	}
	return (0);
}

void	free_filenames(t_data *data)
{
	if (data->file_name)
		free_null(1, data->file_name);
	if (data->file_name2)
		free_null(1, data->file_name2);
	if (data->file_name_append)
		free_null(1, data->file_name_append);
}
