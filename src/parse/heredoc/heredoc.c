/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 15:49:27 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	heredoc_prompt(t_data *data)
{
	char	*hd_tmp;
	char	*hd_tmp_i;
	int		hd_fd;
	char	*line;

	hd_tmp_i = ft_itoa(data->heredoc_index);
	hd_tmp = ft_strjoin(".heredoc_tmp", hd_tmp_i);
	hd_fd = open(hd_tmp, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	free(hd_tmp);
	free(hd_tmp_i);
	line = ft_strdup("42");
	while (ft_strncmp(data->hdoc.delim, line, ft_strlen(data->hdoc.delim)))
	{
		free(line);
		line = get_next_line(0);
		line[ft_strlen(line)] = '\0';
		if (ft_strncmp(data->hdoc.delim, line, ft_strlen(line)))
			write(hd_fd, line, ft_strlen(line));
	}
	free(line);
	close (hd_fd);
}

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
	wr_tmp_file(data, cmd);
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
	while (cmd[k] == ' ')
		k--;
	data->hdoc.cmd_begin = ft_substr(cmd, j + 1, k - j - 1);
	data->hdoc.cmd_end = ft_strdup(cmd + k);
	wr_tmp_file(data, cmd);
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
	wr_tmp_file(data, cmd);
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
