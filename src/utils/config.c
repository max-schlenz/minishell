/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:17:45 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/08 19:07:43 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	check if folder is writable for config file and create file if neccessary
static void	check_cfg(t_data *data, bool fallback)
{
	int		fd;
	char	*home;

	if (!fallback)
		data->cfg = ft_strdup(CFG);
	else
	{
		home = expand_get_var_content(data, "$HOME");
		data->cfg = realloc_ptr(data->cfg, ft_strjoin(home, "/.mscfg"), true);
		free_null (1, &home);
	}
	if (access(data->cfg, F_OK))
	{
		fd = open(data->cfg, O_CREAT | O_RDWR, 0644);
		if (!fd || access(data->cfg, F_OK))
		{
			if (!fallback)
				check_cfg(data, true);
			else
				cleanup(data, E_RW);
		}
		write(fd, "COLOR=bc\n", 9);
		close (fd);
	}
}

//	parse persistent history
void	read_cfg_history(t_data *data, char *read_buf)
{
	char	*history;

	history = ft_strtrim(read_buf + 8, "\n");
	add_history(history);
	if (data->last_cmd)
		free (data->last_cmd);
	data->last_cmd = ft_strdup(history);
	free (history);
}

//	reading and parsing of config file
void	read_cfg(t_data *data)
{
	char	*read_buf;

	check_cfg(data, false);
	if (!access(data->cfg, F_OK))
	{
		data->mscfg = open(data->cfg, O_RDWR | O_APPEND, 0644);
		if (!data->mscfg || access(data->cfg, F_OK))
			cleanup(data, E_RW);
		read_buf = ft_strdup("42");
		while (read_buf != NULL)
		{
			free(read_buf);
			read_buf = get_next_line(data->mscfg);
			if (read_buf && read_buf[0] == '\n')
				continue ;
			else if (read_buf && !ft_strncmp(read_buf, "COLOR", 5))
				builtin_color(data, read_buf + strlen_var(read_buf) + 1);
			else if (read_buf && !ft_strncmp(read_buf, "HISTORY", 7))
				read_cfg_history(data, read_buf);
			else
				continue ;
		}
	}
	else
		cleanup(data, E_RW);
}

int	strdiff(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
	i++;
	}
	if ((!s1[i] && s2[i]) || (!s2[i] && s1[i]))
		return (1);
	return (0);
}
