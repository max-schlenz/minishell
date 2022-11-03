/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_match.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 14:36:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 13:58:30 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	match_init(t_data *data, int *indexes)
{
	if (*indexes == 0)
		data->wcmatch.left = true;
	else
		data->wcmatch.left = false;
	data->wcmatch.prev_index = 0;
	data->wcmatch.i = 0;
}

static int	get_sub_word(t_data *data, char *to_ext, int *indexes, char *file)
{
	data->wcmatch.word_len = (*indexes) - data->wcmatch.prev_index - 1;
	if (!data->wcmatch.prev_index)
		data->wcmatch.word_len = (*indexes);
	if (*(indexes + 1) == -1 && data->wcmatch.left)
		data->wcmatch.sub_word = ft_substr(to_ext, (*indexes) + 1,
				ft_strlen(to_ext) - 1);
	else if (*(indexes + 1) != -1 && data->wcmatch.left)
		data->wcmatch.sub_word = ft_substr(to_ext, (*indexes) + 1,
				*(indexes + 1) - *(indexes) - 1);
	else if (data->wcmatch.prev_index == 0 && !data->wcmatch.left)
		data->wcmatch.sub_word = ft_substr(to_ext, 0, data->wcmatch.word_len);
	else if (!data->wcmatch.left)
		data->wcmatch.sub_word = ft_substr(to_ext, data->wcmatch.prev_index + 1,
				data->wcmatch.word_len);
	if (!file[data->wcmatch.i] && (*data->wcmatch.sub_word))
		return (free(data->wcmatch.sub_word), 0);
	else if (*(indexes) == (int)ft_strlen(to_ext) - 1 && data->wcmatch.left)
		return (free(data->wcmatch.sub_word), 1);
	return (2);
}

static bool	match_helper(t_data *data, int *indexes, char *file)
{
	if (*(indexes + 1) == -1 && data->wcmatch.left)
	{
		if (!match_end(data, file, data->wcmatch.sub_word))
			return (free(data->wcmatch.sub_word), false);
	}
	else if (data->wcmatch.prev_index == 0 && !data->wcmatch.left)
	{
		if (!match_start(data, file, data->wcmatch.sub_word))
			return (free(data->wcmatch.sub_word), false);
	}
	else if (data->wcmatch.left)
	{
		if (!match_inbetween(data, file, data->wcmatch.sub_word))
			return (free(data->wcmatch.sub_word), false);
	}
	else
	{
		if (!match_inbetween(data, file, data->wcmatch.sub_word))
			return (free(data->wcmatch.sub_word), false);
	}
	return (true);
}

static bool	match(t_data *data, char *to_ext, int *indexes, char *file)
{
	int	flag;

	match_init(data, indexes);
	while (*indexes != -1)
	{
		flag = get_sub_word(data, to_ext, indexes, file);
		if (!flag)
			return (false);
		else if (flag == 1)
			return (true);
		else if (!match_helper(data, indexes, file))
			return (false);
		data->wcmatch.prev_index = *indexes;
		indexes++;
		free(data->wcmatch.sub_word);
	}
	if (to_ext[*(indexes - 1) + 1] != '\0')
	{
		data->wcmatch.sub_word = ft_substr(to_ext, *(indexes - 1) + 1,
				ft_strlen(to_ext) - *(indexes - 1) - 1);
		if (!match_end(data, file, data->wcmatch.sub_word))
			return (free(data->wcmatch.sub_word), false);
		free(data->wcmatch.sub_word);
	}
	return (true);
}

char	**match_files(t_data *data, char *to_be_extended, int *indexes)
{
	char			**files;
	int				i;
	DIR				*dirp;
	struct dirent	*dirent;

	dirp = opendir(".");
	files = (char **)ft_calloc(sizeof(char *), enum_files("."));
	if (!files)
		cleanup(data, E_MEM);
	i = 0;
	dirent = readdir(dirp);
	while (dirent)
	{
		if (match(data, to_be_extended, indexes, dirent->d_name))
		{
			if (dirent->d_name[0] != '.')
				files[i++] = ft_strdup(dirent->d_name);
		}
		dirent = readdir(dirp);
	}
	files[i] = NULL;
	closedir(dirp);
	return (files);
}
