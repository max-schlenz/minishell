/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:31:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 13:59:26 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// static bool	setup_redir_files(t_data *data, char *cmd, int *i)
// {
// 	if (!data->flags->redir_out)
// 	{
// 		printf("hello\n");
// 		if (!setup_filenames(data, i, cmd, 0))
// 			return (true);
// 	}
// 	else
// 	{
// 		if (!setup_filenames(data, i, cmd, 1))
// 			return (true);
// 	}
// 	return (false);
// }

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
	
	if (setup_argv_set_redir_flags(data, cmd, i))
	{
		return (true);
	}
	// data->argv[data->parser.array_index] = NULL;
	if ((*i) < ft_strlen(cmd))
	{
		(*i)++;
		if (setup_pipe(data, cmd, i))
			return (true);
	}
	data->parser.arg_start = (*i);
	if (cmd[*i] && cmd[*i] == '>')
	{
		setup_redir_out(data);
		return (false);
	}
	return (false);
}

bool	setup_redir(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_dquote && !data->flags->f_squote && !data->flags->f_esc
		&& (cmd[*i] == '>' || cmd[*i] == '<'))
	{
		if (setup_redir_util(data, cmd, i))
			return (true);
	}
	return (false);
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
