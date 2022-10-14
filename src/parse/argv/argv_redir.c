/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:31:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 22:54:37 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	split_redir_files(t_data *data, char *cmd, int *i)
{
	if (!data->flags->redir_out)
	{
		if (!set_filenames(data, i, cmd, 0))
			return (true);
	}
	else
	{
		if (!set_filenames(data, i, cmd, 1))
			return (true);
	}
	return (false);
}

// data->flags->redir_in = false;
static bool	split_redir_redir_out(t_data *data)
{
	int		tmp_fd;

	data->flags->redir_out = false;
	data->flags->redir_append = false;
	if (data->file_name2)
	{
		tmp_fd = open(data->file_name2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		close(tmp_fd);
	}
	return (false);
}

static bool	split_redir_util(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote
		&& !ft_strncmp(cmd + (*i), ">>", 2))
		return (parse_redir_out(data, cmd, i));
	else if (cmd[*i] == '>')
		data->flags->redir_out = true;
	else if (cmd[*i] == '<')
		data->flags->redir_in = true;
	(*i) += 2;
	data->argv[data->parser.array_index] = NULL;
	if (split_redir_files(data, cmd, i))
		return (true);
	(*i)++;
	if (cmd[*i] && cmd[*i] == '|')
		data->flags->pipe = true;
	if (cmd[*i] && cmd[*i] == '>')
	{
		split_redir_redir_out(data);
		return (false);
	}
	return (true);
}

int	split_redir(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc
		&& (cmd[*i] == '>' || cmd[*i] == '<'))
	{
		if (split_redir_util(data, cmd, i))
			return (1);
		else
			return (2);
	}
	return (0);
}
