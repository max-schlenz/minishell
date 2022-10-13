/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/05 12:16:48 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	wr_tmp_file(t_data *data, char *cmd)
{
	heredoc(data);
	data->heredoc.hd_tmp_i = ft_itoa(data->heredoc_index);
	data->heredoc.hd_tmp = ft_strjoin(".heredoc_tmp", data->heredoc.hd_tmp_i);
	if (data->heredoc.andor)
		data->heredoc.cmd_redir = ft_strjoin(data->heredoc.cmd, " < ");
	else
		data->heredoc.cmd_redir = ft_strjoin(data->heredoc.cmd_begin, " < ");
	data->heredoc.cmd_redir_file
		= ft_strjoin(data->heredoc.cmd_redir, data->heredoc.hd_tmp);
	if (data->heredoc.andor)
	{
		data->heredoc.cmd_done1
			= ft_strjoin(data->heredoc.cmd_redir_file, data->heredoc.cmd_end);
		data->heredoc.cmd_done
			= ft_strjoin(data->heredoc.cmd_begin, data->heredoc.cmd_done1);
	}
	else
	{
		data->heredoc.cmd_done
			= ft_strjoin(data->heredoc.cmd_redir_file, data->heredoc.cmd_end);
	}
	data->heredoc.andor = false;
}

void	wr_new_cmd(t_data *data, char **cmd, int *i)
{
	free (*cmd);
	*cmd = ft_strdup(data->heredoc.cmd_done);
	data->heredoc_index++;
	*i = 0;
	data->heredoc.hd = false;
}

void	init_hd(t_data *data)
{
	data->heredoc.cmd_begin = NULL;
	data->heredoc.cmd = NULL;
	data->heredoc.cmd_end = NULL;
	data->heredoc.cmd_redir = NULL;
	data->heredoc.cmd_redir_file = NULL;
	data->heredoc.cmd_done = NULL;
	data->heredoc.cmd_done1 = NULL;
	data->heredoc.delim = NULL;
	data->heredoc.hd_tmp = NULL;
	data->heredoc.hd_tmp_i = NULL;
}

void	free_hd(t_data *data)
{
	if (data->heredoc.cmd_begin)
		free(data->heredoc.cmd_begin);
	if (data->heredoc.cmd)
		free(data->heredoc.cmd);
	if (data->heredoc.cmd_end)
		free(data->heredoc.cmd_end);
	if (data->heredoc.cmd_redir)
		free(data->heredoc.cmd_redir);
	if (data->heredoc.cmd_redir_file)
		free(data->heredoc.cmd_redir_file);
	if (data->heredoc.cmd_done)
		free(data->heredoc.cmd_done);
	if (data->heredoc.cmd_done1)
		free(data->heredoc.cmd_done1);
	if (data->heredoc.delim)
		free(data->heredoc.delim);
	if (data->heredoc.hd_tmp)
		free(data->heredoc.hd_tmp);
	if (data->heredoc.hd_tmp_i)
		free(data->heredoc.hd_tmp_i);
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
