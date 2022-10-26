/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/26 13:21:53 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	rm_tmp_files(t_data *data)
{
	int		i;
	char	*tmp_name;
	char	*tmp_index;

	i = 0;
	while (i < data->heredoc_index)
	{
		tmp_index = ft_itoa(i);
		tmp_name = ft_strjoin(".heredoc_tmp", tmp_index);
		unlink(tmp_name);
		free_str (2, tmp_name, tmp_index);
		i++;
	}
}

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

char	*handle_heredoc(t_data *data, char *cmd)
{
	int		i;
	int		j;
	char	*cmd_before;
	char	*cmd_after;

	i = 0;
	j = 0;
	while (cmd && cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			data->hdoc.quote = !data->hdoc.quote;
		else if (!data->hdoc.quote && !ft_strncmp(cmd + i, "<<", 2))
		{
			j = i + 3;
			cmd_before = ft_substr(cmd, 0, i);
			while (cmd[j] && cmd[j] != ' ')
				j++;
			data->hdoc.delim = ft_substr(cmd, i + 3, j - (i + 3));
			cmd_after = ft_strdup(cmd + j);
			wr_tmp_file(data);
			cmd = str_realloc(cmd,
					merge_str(3, cmd_before, data->hdoc.hd_tmp, cmd_after), 1);
		}
		i++;
	}
	return (cmd);
}
