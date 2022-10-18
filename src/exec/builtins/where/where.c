/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   where.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 23:32:59 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 11:36:57 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	*where_get_path(t_data *data, char *cmd)
{
	int		i;
	char	*abs_path_tmp;
	char	*abs_path;
	char	*cmd_trim;

	i = 0;
	while (data->path && data->path[i])
	{
		if (ft_strlen(cmd) > 2 && ft_strchr(cmd + 2, '/'))
			return (NULL);
		abs_path_tmp = ft_strjoin(data->path[i], "/");
		cmd_trim = ft_strtrim(cmd, " ");
		abs_path = merge_str(2, abs_path_tmp, cmd_trim);
		if (!access(abs_path, F_OK) && check_path(abs_path))
			printf("%s\n", abs_path);
		free (abs_path);
		i++;
	}
	return (NULL);
}

bool	builtin_where(t_data *data)
{
	int	i;
	int	index_arg;

	i = 0;
	index_arg = 1;
	while (data->argv[index_arg])
		where_get_path(data, data->argv[index_arg++]);
	return (true);
}
