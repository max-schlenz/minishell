/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 11:02:25 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/12 16:48:21 by mschlenz         ###   ########.fr       */
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
	char	*ret_tmp;
	
	ret = NULL;
	ret_tmp = NULL;
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
	i = 0;
	while (tmp[i])
	{
		ret_tmp = ft_strjoin_dup(ret, tmp[i++]);
		free(ret);
		ret = ft_strjoin(ret_tmp, " ");
		free(ret_tmp);
	}
	return (ret);
}

char	*wildcard_begin(t_data *data, char *cmd)
{
	char 	**tmp;
	pid_t	pid;
	int 	i = 0;
	int 	j = 0;
	int 	past_wc = 0;
	int		post_wc = 0;
	int 	len_arg = 0;
	char	*ret;
	char	*ret_tmp;
	

	int len_tmp = 0;
	char *tmp_content = NULL;
	
	ret = NULL;
	ret_tmp = NULL;
	while (cmd[past_wc] && cmd[past_wc] != ' ')
		past_wc++;
	// printf("%d\n", past_wc);
	// exit(0);
	read_dir_content(data);
	while (data->wc_dir_content[i])
	{
		len_tmp = ft_strlen(data->wc_dir_content[i]);
			printf("%s\n", data->wc_dir_content[i] + past_wc);
		if (len_tmp > len_tmp && !ft_strncmp(cmd + past_wc, data->wc_dir_content[i] + past_wc, past_wc))
			j++;
		i++;
	}
	exit(0);
	tmp = ft_calloc(j, sizeof(char *));
	i = 0;
	j = 0;
	while (data->wc_dir_content[i])
	{
		if (!ft_strncmp(data->wc_dir_content[i], cmd, past_wc))
			tmp[j++] = ft_strdup(data->wc_dir_content[i]);
		i++;
	}
	tmp[j] = NULL;
	i = 0;
	while (tmp[i])
	{
		ret_tmp = ft_strjoin_dup(ret, tmp[i++]);
		free(ret);
		ret = ft_strjoin(ret_tmp, " ");
		free(ret_tmp);
	}
	return (ret);
}

char	*find_wc(t_data *data, char *cmd)
{
	int		i = 0;
	char	*str_before_wc;
	char	*expanded_wcs;
	char	*rest;
	char	*cmd_tmp;
	
	while (cmd[i])
	{
		if (cmd[i + 1] && cmd[i] == ' '&& cmd[i + 1] == '*')
		{
			str_before_wc = ft_substr(cmd, 0, i + 1);
			while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '*'))
				i++;
			// printf("%s\n", str_before_wc);
			expanded_wcs = wildcard_begin(data, cmd + i);
			rest = ft_strdup(cmd + i);
			while (cmd[--i] && cmd[i] != ' ')
				i--;
			cmd_tmp = ft_strjoin(str_before_wc, expanded_wcs);
			free(str_before_wc);
			free(expanded_wcs);
			cmd = ft_strjoin(cmd_tmp, rest);
			free(cmd_tmp);
		}
		else if (cmd[i] == '*')
		{
			while (cmd[i] && cmd[i] != ' ')
				i++;
			rest = ft_strdup(cmd + i);
			while (cmd[--i] && cmd[i] != ' ')
				i--;
			str_before_wc = ft_substr(cmd, 0, i + 1);
			expanded_wcs = wildcard(data, cmd + i);
			cmd_tmp = ft_strjoin(str_before_wc, expanded_wcs);
			free(str_before_wc);
			free(expanded_wcs);
			cmd = ft_strjoin(cmd_tmp, rest);
			free(cmd_tmp);
		}
	i++;
	}
	return (cmd);
}