/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/04 16:25:16 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	heredoc_andor(t_data *data, char *cmd, int i)
{
	int	j;
	int	k;

	data->heredoc.hd = true;
	free_hd(data);
	data->heredoc.cmd_begin = ft_substr(cmd, 0, i + 3);
	i += 6;
	j = i;
	while (cmd[j] && cmd[j] != ' ')
        j++;
	data->heredoc.delim_tmp = ft_substr(cmd, i, j - i);
	data->heredoc.delim = ft_strjoin(data->heredoc.delim_tmp, "\n");
	k = j + 1;
	while (cmd[k] && cmd[k] != ' ')
		k++;
	data->heredoc.cmd = ft_substr(cmd, j + 1, k - j - 1);
	data->heredoc.cmd_end = ft_strdup(cmd + k);
	data->heredoc.andor = true;
	wr_tmp_file(data, cmd);
}

static void	heredoc_begin(t_data *data, char *cmd, int i)
{
	int	j;
	int	k;

	data->heredoc.hd = true;
	free_hd(data);
	i += 3;
	j = i;
	while (cmd[j] && cmd[j] != ' ')
		j++;
	data->heredoc.delim_tmp = ft_substr(cmd, i, j - i);
	data->heredoc.delim = ft_strjoin(data->heredoc.delim_tmp, "\n");
	k = j + 1;
	while (cmd[k]
		&& cmd[k] != '<'
		&& cmd[k] != '>'
		&& cmd[k] != '|'
		&& cmd[k] != '&'
		&& cmd[k] != ';')
		k++;
	while (cmd[k] == ' ')
		k--;
	data->heredoc.cmd_begin = ft_substr(cmd, j + 1, k - j - 1);
	data->heredoc.cmd_end = ft_strdup(cmd + k);
	wr_tmp_file(data, cmd);
}

static void	heredoc_after_cmd(t_data *data, char *cmd, int i)
{
	int	j;
	int	k;

	data->heredoc.hd = true;
	free_hd(data);
	j = i - 1;
	while (cmd[j] && cmd[j] != ' ')
		j--;
	data->heredoc.cmd_begin = ft_substr(cmd, 0, j);
	i += 3;
	k = i;
	while (cmd[k] && cmd[k] != ' ')
		k++;
	data->heredoc.delim_tmp = ft_substr(cmd, i, k - i);
	data->heredoc.cmd_end = ft_strdup(cmd + k);
	data->heredoc.delim = ft_strjoin(data->heredoc.delim_tmp, "\n");
	wr_tmp_file(data, cmd);
}

char	*handle_heredoc(t_data *data, char *cmd)
{
	int		i;

	i = 0;
	while (cmd && cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			data->heredoc.quote = !data->heredoc.quote;
		else if (!data->heredoc.quote && (!ft_strncmp(cmd + i, "&& <<", 5)
				|| !ft_strncmp(cmd + i, "|| <<", 5)))
			heredoc_andor(data, cmd, i);
		else if (!data->heredoc.quote && !ft_strncmp(cmd + i, "<<", 2))
		{
			if (!i)
				heredoc_begin(data, cmd, i);
			else
				heredoc_after_cmd(data, cmd, i);
		}
		if (data->heredoc.hd)
		{
			wr_new_cmd(data, &cmd, &i);
			continue ;
		}
		i++;
	}
	return (cmd);
}
