/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 14:11:26 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	heredoc_wr_tmp_file(t_data *data)
{
	char	*path;
	char	*home;

	heredoc_prompt(data);
	data->hdoc.hd_tmp_i = ft_itoa(data->heredoc_index);
	path = ft_strjoin(".heredoc_tmp", data->hdoc.hd_tmp_i);
	if (!access(path, F_OK))
		data->hdoc.hd_tmp = ft_strjoin("< .heredoc_tmp", data->hdoc.hd_tmp_i);
	else
	{
		home = expand_get_var_content(data, "~");
		home = realloc_ptr(home, ft_strjoin("< ", home), true);
		path = realloc_ptr(path, ft_strjoin(home, "/"), true);
		path = realloc_ptr(path, ft_strjoin(path, ".heredoc_tmp"), true);
		data->hdoc.hd_tmp = ft_strjoin(path, data->hdoc.hd_tmp_i);
	}
	data->heredoc_index++;
	free_null(2, &data->hdoc.delim, &data->hdoc.hd_tmp_i);
}

void	heredoc_init(t_data *data)
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

void	heredoc_free(t_data *data)
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
	heredoc_init(data);
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
