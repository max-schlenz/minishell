/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 13:12:54 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	heredoc_andor(t_data *data, char *cmd, int i)
{
	int	j;
	int	k;

	data->hdoc.hd = true;
	free_hd(data);
	data->hdoc.cmd_begin = ft_substr(cmd, 0, i + 3);
	i += 6;
	j = i;
	while (cmd[j] && cmd[j] != ' ')
		j++;
	data->hdoc.delim = heredoc_delim(cmd, i, j);
	k = j + 1;
	while (cmd[k] && cmd[k] != ' ')
		k++;
	data->hdoc.cmd = ft_substr(cmd, j + 1, k - j - 1);
	data->hdoc.cmd_end = ft_strdup(cmd + k);
	data->hdoc.andor = true;
	wr_tmp_file(data);
}

static void	heredoc_begin(t_data *data, char *cmd, int i)
{
	int	j;
	int	k;

	data->hdoc.hd = true;
	free_hd(data);
	i += 3;
	j = i;
	while (cmd[j] && cmd[j] != ' ')
		j++;
	data->hdoc.delim = heredoc_delim(cmd, i, j);
	k = j + 1;
	while (cmd[k]
		&& cmd[k] != '<'
		&& cmd[k] != '>'
		&& cmd[k] != '|'
		&& cmd[k] != '&'
		&& cmd[k] != ';')
		k++;
	if (cmd[k - 1] == ' ')
		k--;
	data->hdoc.cmd_begin = ft_substr(cmd, j + 1, k - j - 1);
	data->hdoc.cmd_end = ft_strdup(cmd + k);
	wr_tmp_file(data);
}

static void	heredoc_after_cmd(t_data *data, char *cmd, int i)
{
	int	j;
	int	k;

	data->hdoc.hd = true;
	free_hd(data);
	j = i - 1;
	while (cmd[j] && cmd[j] != ' ')
		j--;
	data->hdoc.cmd_begin = ft_substr(cmd, 0, j);
	i += 3;
	k = i;
	while (cmd[k] && cmd[k] != ' ')
		k++;
	data->hdoc.delim = heredoc_delim(cmd, i, k);
	data->hdoc.cmd_end = ft_strdup(cmd + k);
	wr_tmp_file(data);
}

char	*handle_heredoc(t_data *data, char *cmd)
{
	int		i;

	i = 0;
	while (cmd && cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			data->hdoc.quote = !data->hdoc.quote;
		else if (!data->hdoc.quote && (!ft_strncmp(cmd + i, "&& <<", 5)
				|| !ft_strncmp(cmd + i, "|| <<", 5)))
			heredoc_andor(data, cmd, i);
		else if (!data->hdoc.quote && !ft_strncmp(cmd + i, "<<", 2))
		{
			if (!i)
				heredoc_begin(data, cmd, i);
			else
				heredoc_after_cmd(data, cmd, i);
		}
		if (data->hdoc.hd)
		{
			wr_new_cmd(data, &cmd, &i);
			continue ;
		}
		i++;
	}
	return (cmd);
}
