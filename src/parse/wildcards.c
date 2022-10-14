/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 17:26:31 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/14 23:04:28 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

char	**realloc_argv(t_data *data, int argv_i, char *filename, int overwrite)
{
	char	**new_argv;
	int		i;
	int		j;

	new_argv = (char **)ft_calloc(sizeof(char *), len_argv(data->argv) + 2);
	i = 0;
	j = 0;
	while (data->argv[i])
	{
		new_argv[j] = ft_strdup(data->argv[i]);
		if (j == argv_i && overwrite)
		{
			free(new_argv[j]);
			new_argv[j] = ft_strdup(filename);
		}
		else if (j == argv_i && !overwrite)
			new_argv[++j] = ft_strdup(filename);
		free(data->argv[i]);
		i++;
		j++;
	}
	new_argv[j] = NULL;
	free(data->argv);
	return (new_argv);
}

char	*delete_str(char *cmd, size_t start, size_t end)
{
	char		*ret;
	size_t		len;
	size_t		i;
	int			j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len - (start - end) + 1);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (i == start)
			i = end - 1;
		ret[j++] = cmd[i++];
	}
	ret[j] = '\0';
	return (ret);
}

int	num_wildcards(char *str)
{
	int	counter;

	counter = 0;
	while (*str)
	{
		if (*str == '*')
			counter++;
		str++;
	}
	return (counter);
}

int	*get_indexes(char *str)
{
	int	i;
	int	j;
	int	*indexes;
	int	*tmp;

	i = 0;
	j = 0;
	indexes = (int *)ft_calloc(sizeof(int), num_wildcards(str) + 1);
	while (str[i])
	{
		if (str[i] == '*')
		{
			indexes[j] = i;
			j++;
		}
		i++;
	}
	tmp = indexes;
	tmp += j;
	*tmp = -1;
	return (indexes);
}

// int	i_len(int *i)
// {
// 	int	counter;

// 	counter = 0;
// 	while (i + counter)
// 		counter++;
// 	return (counter);
// }

static bool	end(char *str, char *pattern, int *i)
{
	int	index;

	if (!*str && *pattern != '\0')
		return (false);
	index = ft_strlen(str) - ft_strlen(pattern);
	while (*pattern && *pattern != '*')
	{
		if (str[index] != *pattern)
			return (false);
		index++;
		pattern++;
	}
	(*i) = ft_strlen(str);
	return (true);
}

/*static bool start(char *str, char *pattern, int *file_index)
{	
	int	i;
	int	j;

	i = 0;
	while (str[*file_index])
	{
		j = 0;
		while (pattern[j] && pattern[j] != '*')
		{
			if (str[i] != pattern[j])
				break ;
			i++;
			j++;
		}
		if (!pattern[j])
			break ;
		(*file_index)++;
		i = (*file_index);
	}
	(*file_index) = i;
	if (pattern[j])
		return (false);
	return (true);
}*/


static bool	inbetween(char *str, char *pattern, int *file_index)
{
	int	i;
	int	j;

	while (str[*file_index])
	{
		j = 0;
		i = (*file_index);
		while (pattern[j] && pattern[j] != '*')
		{
			if (str[i] != pattern[j])
				break ;
			i++;
			j++;
		}
		if (!pattern[j])
			break ;
		(*file_index)++;
	}
	(*file_index) += j;
	if (pattern[j])
		return (false);
	return (true);
}

static bool	start(char *str, char *pattern, int *file_index)
{
	while (*pattern)
	{
		if (str[*file_index] != *pattern)
			return (false);
		(*file_index)++;
		pattern++;
	}
	return (true);
}

static bool	match(char *to_be_extended, int *indexes, char *file)
{
	int		word_len;
	int		prev_index;
	char	*sub_word;
	int		i;
	int		left;

	prev_index = 0;
	i = 0;
	left = 0;
	if (*indexes == 0)
		left = 1;
	while (*indexes != -1)
	{
		word_len = (*indexes) - prev_index - 1;
		if (!prev_index)
			word_len = (*indexes);
		if (*(indexes + 1) == -1 && left)
			sub_word = ft_substr(to_be_extended, (*indexes) + 1, ft_strlen(to_be_extended) - 1);
		else if (*(indexes + 1) != -1 && left)
			sub_word = ft_substr(to_be_extended, (*indexes) + 1, *(indexes + 1) - *(indexes) - 1);
		else if (prev_index == 0 && !left)
		{
			sub_word = ft_substr(to_be_extended, 0, word_len);
		}
		else if (!left)
		{
			sub_word = ft_substr(to_be_extended, prev_index + 1, word_len);	
		}

		if (!file[i] && *sub_word)
		{
				free(sub_word);
				return (false);
		}
		else if (*(indexes) == (int)ft_strlen(to_be_extended) - 1 && left)
		{
			free(sub_word);
			return (true);
		}
		else if (*(indexes + 1) == -1 && left)
		{
			if (!end(file, sub_word, &i))
			{
				free(sub_word);
				return (false);
			}
		}
		else if (prev_index == 0 && !left)
		{
			if (!start(file, sub_word, &i))
			{
				free(sub_word);
				return (false);
			}
		}
		else if (left)
		{
			if (!inbetween(file, sub_word, &i))
			{
				free(sub_word);
				return (false);
			}
		}
		else
		{
			if (!inbetween(file, sub_word, &i))
			{
				free(sub_word);
				return (false);
			}
		}
		prev_index = *indexes;
		indexes++;
		free(sub_word);
	}
	if (to_be_extended[*(indexes - 1) + 1] != '\0')
	{
		sub_word = ft_substr(to_be_extended, *(indexes - 1) + 1, ft_strlen(to_be_extended) - *(indexes - 1) - 1);
		if (!end(file, sub_word, &i))
		{		
				free(sub_word);
				return (false);
		}
		free(sub_word);
	}
	return (true);
}

static char	**match_files(char *to_be_extended, int *indexes)
{
	char	**files = (char **)ft_calloc(sizeof(char *), 100);
	int		i;
	DIR 	*dirp= opendir(".");
    struct 	dirent *direntStruct;
	i = 0;
	while ((direntStruct = readdir(dirp)))
	{
		if (match(to_be_extended, indexes, direntStruct->d_name))
		{
			//printf("filename, %s\n", direntStruct->d_name);
			if (direntStruct->d_name[0] != '.')
				files[i++] = ft_strdup(direntStruct->d_name);
		}
	}
	files[i] = NULL;
	closedir(dirp);
	return (files);
}

static void	extend_wildcards(t_data *data, char *to_be_extended, char **files, int argv_i)
{
	while (*files)
	{
		if (!ft_strncmp(data->argv[argv_i], to_be_extended, ft_strlen(to_be_extended) + 1))
		{
			data->argv = realloc_argv(data, argv_i, *files, 1);
		}
		else
		{
			data->argv = realloc_argv(data, argv_i, *files, 0);
			argv_i++;
		}
		files++;
	}
}

void	get_all_names(t_data *data)
{
	int		i;
	int		*indexes;
	char	*to_be_extended;
	char	**files;

	i = 0;
	while (data->argv[i])
	{
		data->argv[i] = skip_d(data, data->argv[i], '*');
		indexes = get_indexes(data->argv[i]);
		if (*indexes != -1)
		{
			to_be_extended = ft_strdup(data->argv[i]);
			files = match_files(to_be_extended, indexes);
			extend_wildcards(data, to_be_extended, files, i);
			free_array(files);
			free(files);
			free(to_be_extended);
		}
		free(indexes);
		i++;
	}
}