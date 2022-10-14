/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 23:04:49 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	wr_tmp_file(t_data *data)
{
	heredoc_prompt(data);
	data->hdoc.hd_tmp_i = ft_itoa(data->heredoc_index);
	data->hdoc.hd_tmp = ft_strjoin(".heredoc_tmp", data->hdoc.hd_tmp_i);
	if (data->hdoc.andor)
		data->hdoc.cmd_redir = ft_strjoin(data->hdoc.cmd, " < ");
	else
		data->hdoc.cmd_redir = ft_strjoin(data->hdoc.cmd_begin, " < ");
	data->hdoc.cmd_redir_file
		= ft_strjoin(data->hdoc.cmd_redir, data->hdoc.hd_tmp);
	if (data->hdoc.andor)
	{
		data->hdoc.cmd_done1
			= ft_strjoin(data->hdoc.cmd_redir_file, data->hdoc.cmd_end);
		data->hdoc.cmd_done
			= ft_strjoin(data->hdoc.cmd_begin, data->hdoc.cmd_done1);
	}
	else
	{
		data->hdoc.cmd_done
			= ft_strjoin(data->hdoc.cmd_redir_file, data->hdoc.cmd_end);
	}
	data->hdoc.andor = false;
}

void	wr_new_cmd(t_data *data, char **cmd, int *i)
{
	free (*cmd);
	*cmd = ft_strdup(data->hdoc.cmd_done);
	data->heredoc_index++;
	*i = 0;
	data->hdoc.hd = false;
}

void	init_hd(t_data *data)
{
	data->hdoc.cmd_begin = NULL;
	data->hdoc.cmd = NULL;
	data->hdoc.cmd_end = NULL;
	data->hdoc.cmd_redir = NULL;
	data->hdoc.cmd_redir_file = NULL;
	data->hdoc.cmd_done = NULL;
	data->hdoc.cmd_done1 = NULL;
	data->hdoc.delim = NULL;
	data->hdoc.hd_tmp = NULL;
	data->hdoc.hd_tmp_i = NULL;
}

void	free_hd(t_data *data)
{
	if (data->hdoc.cmd_begin)
		free(data->hdoc.cmd_begin);
	if (data->hdoc.cmd)
		free(data->hdoc.cmd);
	if (data->hdoc.cmd_end)
		free(data->hdoc.cmd_end);
	if (data->hdoc.cmd_redir)
		free(data->hdoc.cmd_redir);
	if (data->hdoc.cmd_redir_file)
		free(data->hdoc.cmd_redir_file);
	if (data->hdoc.cmd_done)
		free(data->hdoc.cmd_done);
	if (data->hdoc.cmd_done1)
		free(data->hdoc.cmd_done1);
	if (data->hdoc.delim)
		free(data->hdoc.delim);
	if (data->hdoc.hd_tmp)
		free(data->hdoc.hd_tmp);
	if (data->hdoc.hd_tmp_i)
		free(data->hdoc.hd_tmp_i);
	init_hd(data);
}

char	*heredoc_delim(char *cmd, int i, int j)
{
	char	*delim;
	char	*delim_nl;

	if (!cmd)
		return (ft_strdup("\n"));
	delim = NULL;
	delim_nl = NULL;
	delim = ft_substr(cmd, i, j - i);
	delim_nl = ft_strjoin_dup(delim, "\n");
	free (delim);
	return (delim_nl);
}
