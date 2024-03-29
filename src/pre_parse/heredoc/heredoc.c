/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/11 14:52:14 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	unlinks heredoc tmp files
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
		tmp_home = expand_get_var_content(data, "~");
		tmp_path = ft_strjoin(tmp_home, "/");
		tmp_path
			= realloc_ptr(tmp_path, ft_strjoin(tmp_path, ".heredoc_tmp"), true);
		tmp_path = realloc_ptr(tmp_path, ft_strjoin(tmp_path, tmp_index), true);
		unlink(tmp_name);
		unlink(tmp_path);
		free_null (4, &tmp_name, &tmp_index, &tmp_path, &tmp_home);
		i++;
	}
}

static void	handle_heredoc_save_delim(t_data *data, char *cmd, int i, int j)
{
	data->hdoc.delim = ft_substr(cmd, i + 3, j - (i + 3));
	data->hdoc.delim = realloc_ptr(data->hdoc.delim,
			ft_strjoin(data->hdoc.delim, "\n"), true);
}

//	resolves heredoc
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
			handle_heredoc_save_delim(data, cmd, i, j);
			cmd_after = ft_strdup(cmd + j);
			heredoc_wr_tmp_file(data);
			cmd = realloc_ptr(cmd,
					merge_str(3, cmd_before, data->hdoc.hd_tmp, cmd_after), 1);
		}
		i++;
	}
	return (cmd);
}
