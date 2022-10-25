/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/25 16:22:45 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*heredoc_get_cmd(char *str)
{
	size_t		i;

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
	if (i <= ft_strlen(str))
		return (ft_strdup(str + i));
	else
		return (NULL);
}

void	heredoc_set_flags(t_data *data)
{
	data->heredoc_index++;
	data->flags->heredoc = false;
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
