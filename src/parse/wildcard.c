/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 11:02:25 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/12 11:52:51 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	read_dir_content(t_data *data)
{
	DIR *dir;
	struct dirent *dirent;
	int	i;

	i = 1;
	dir = opendir(".");
	dirent = readdir(dir);
	while (dirent)
	{
		i++;
		dirent = readdir(dir);
	}
	closedir(dir);
	data->wc_dir_content = ft_calloc(i, sizeof(char *));
	dir = opendir(".");
	dirent = readdir(dir);
	i = 0;
	while (dirent)
	{
		data->wc_dir_content[i++] = ft_strdup(dirent->d_name);
		dirent = readdir(dir);
	}
	closedir(dir);
	// i = 0;
	// while (data->wc_dir_content[i])
	// 	printf("%s\n", data->wc_dir_content[i++]);
}


char	*wildcard(t_data *data, char *cmd)
{
	char 	**tmp;
	pid_t	pid;
	int 	i = 0;
	int 	j = 0;
	int 	pre_wc = 0;
	int		post_wc = 0;
	int 	len_arg = 0;
	char	*ret;
	
	ret = NULL;
	while (*cmd == ' ')
		cmd++;
	while (cmd[pre_wc])
	{
		if (cmd[pre_wc] == '*')
			break;
		pre_wc++;
	}
	read_dir_content(data);
	while (data->wc_dir_content[i])
	{
		if (!ft_strncmp(cmd, data->wc_dir_content[i], pre_wc))
			j++;
		i++;
	}
	tmp = ft_calloc(j, sizeof(char *));
	i = 0;
	j = 0;
	while (data->wc_dir_content[i])
	{
		if (!ft_strncmp(data->wc_dir_content[i], cmd, pre_wc))
			tmp[j++] = ft_strdup(data->wc_dir_content[i]);
		i++;
	}
	tmp[j] = NULL;
	// len_arg = ft_strlen(data->argv[i_arg]);
	// post_wc = pre_wc;
	// while (data->argv[i_arg][post_wc])
	// 	post_wc++;
	int 	k = 0;
	while (tmp[k])
		printf("%s\n", tmp[k++]);
	exit(0);
}

char	*find_wc(t_data *data, char *cmd)
{
	int		i = 0;
	char	*tmp;
	
	while (cmd[i])
	{
		if (cmd[i] == '*')
		{
			while (cmd[i] && cmd[i] != ' ')
				i--;
			tmp = wildcard(data, cmd + i);
			i = 0;
			continue;
		}
	i++;
	}
	return (cmd);
}