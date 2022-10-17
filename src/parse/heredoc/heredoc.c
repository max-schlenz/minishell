/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/17 22:20:27 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*heredoc_get_cmd(char *str)
{
	int		i;

	i = ft_strlen(str) - 1;
	while (i)
	{
		if (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>'
			|| str[i] == ';' )
		{
			i += 2;
			break ;
		}
		i--;
	}
	return (ft_strdup(str + i));
}

static size_t	hd_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	if (str[i - 1] == ' ')
		i--;
	return (i);
}

static void	heredoc_prompt_output(t_data *data)
{
	char	*cmd;

	cmd = heredoc_get_cmd(data->hdoc.cmd_begin);
	write(2, "heredoc \x01\033[0m\x02\x01\033[38;5;239m\x02[", 29);
	write(2, cmd, hd_strlen(cmd));
	write(2, " << ", 5);
	write(2, data->hdoc.delim, ft_strlen(data->hdoc.delim) - 1);
	write(2, "] \x01\033[0;1m\x02>\x01\033[0m\x02 ", 19);
	free (cmd);
}

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
		heredoc_prompt_output(data);
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
