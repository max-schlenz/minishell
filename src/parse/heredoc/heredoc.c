/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 13:47:33 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	rm_tmp_files(t_data *data)
{
	int		i;
	char	*tmp_name;
	char	*tmp_index;
	char	*tmp_home;
	char	*tmp_path;

	i = 0;
	while (i < data->heredoc_index)
	{
		tmp_index = ft_itoa(i);
		tmp_name = ft_strjoin(".heredoc_tmp", tmp_index);
		tmp_home = get_var_content(data, "~");
		tmp_path = strmv(ft_strjoin(tmp_home, "/"));
		tmp_path
			= str_realloc(tmp_path, ft_strjoin(tmp_path, ".heredoc_tmp"), true);
		tmp_path = str_realloc(tmp_path, ft_strjoin(tmp_path, tmp_index), true);
		unlink(tmp_name);
		unlink(tmp_path);
		free_str (4, tmp_name, tmp_index, tmp_path, tmp_home);
		i++;
	}
}

char	*handle_heredoc(t_data *data, char *cmd)
{
	int		i;
	int		j;
	char	*cmd_before;
	char	*cmd_after;

	i = 0;
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
