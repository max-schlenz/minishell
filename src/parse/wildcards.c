/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 17:26:31 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/20 13:22:43 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*realloc_cmd(char *cmd, char *file_name, int insert_start)
// {
// 	int	i = 0;
// 	int	j = 0;
// 	int	len_cmd = ft_strlen(cmd);
// 	int len_file_name = ft_strlen(file_name);
// 	char *new = (char *)ft_calloc(sizeof(char), len_cmd + file_name + 2);
// 	while (cmd[i])
// 	{
// 		if (j == insert_start)
// 		{
// 			while (*file_name)
// 				new[j++] = *file_name++;
// 			new[j] = ' ';
// 		}
// 		new[j] = cmd[i];
// 		i++;
// 		j++;
// 	}
// 	new[j] = '\0';
// 	return (new);
// }

int	len_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

char **realloc_argv(t_data *data, int argv_i, char *filename, int overwrite)
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
			new_argv[j] = ft_strdup(filename);
		else if (j == argv_i && !overwrite)
		{
			new_argv[++j] = ft_strdup(filename);
			//j++;
			//continue;
		}
		free(data->argv[i]);
		i++;
		j++;
	}
	new_argv[j] = NULL;
	free(data->argv);
	return(new_argv);
}

char *delete_str(char *cmd, size_t start, size_t end)
{
	char	*ret;
	size_t		len;
	int		i;
	int		j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len - (start - end) + 1);
	i = 0;
	j = 0;
	while(cmd[i])
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
	return(counter);
}

int	*get_indexes(char *str)
{
	int	i;
	int	j;
	int	len;
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

int	i_len(int *i)
{
	int	counter;

	counter = 0;
	while ((i + counter))
		counter++;
	return (counter);
}

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

static bool	move_file_index(char *file, char *pattern, int *i)
{
	while ((file + *i) && ft_strncmp(file + *i, pattern, ft_strlen(pattern)))
		(*i)++;
	if (!(file + *i))
		return (false);
	return (true);
}

static bool start(char *str, char *pattern, int *file_index)
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
}

static bool inbetween(char *str, char *pattern, int *file_index)
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
	(*file_index) += j;
	if (pattern[j])
		return (false);
	return (true);
}


static bool left_wildcard(char *str, char *pattern, int *file_index)
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

static bool right_wildcard(char *str, char *pattern, int *file_index)
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
	int	word_len;
	int		prev_index;
	char	*sub_word;
	int		i;
	int		left;

	prev_index = 0;
	i = 0;
	left = 0;
	if (*indexes == 0)
		left = 1;
	while(*indexes != -1)
	{
		word_len = (*indexes) - prev_index;
		if (*(indexes + 1) == -1 && left)
			sub_word = ft_substr(to_be_extended, (*indexes) + 1, ft_strlen(to_be_extended) - 1);
		else if (*(indexes + 1) != -1 && left)
			sub_word = ft_substr(to_be_extended, (*indexes) + 1, *(indexes + 1) - *(indexes) - 1);
		else if (*(indexes + 1) == -1 && !left)
			sub_word = ft_substr(to_be_extended, 0, ft_strlen(to_be_extended) - 1);
		else if (*(indexes + 1) != -1 && !left)
			sub_word = ft_substr(to_be_extended, prev_index, word_len);
		if (!file[i] && *sub_word)
			return (false);
		else if ((*(indexes) == ft_strlen(to_be_extended) - 1))
		{
			printf("in here\n");
			return (true);
		}
		else if ((*(indexes + 1) == -1 && left))
		{
			printf("start index %d sub %s i %d file %s\n", *(indexes), sub_word, i, file);
			if (!end(file, sub_word, &i))
				return (false);
			printf("start index %d sub %s i %d file %s\n", *(indexes), sub_word, i, file);
		}
		else if (left)
		{
			//printf("start index %d sub %s i %d file %s\n", *(indexes), sub_word, i, file);
			if (!left_wildcard(file, sub_word, &i))
				return (false);
			//printf("\nafter index %d sub %s i %d file %s\n", *indexes, sub_word, i, file);
		}
		else
		{
			printf("lol\n");
			printf("start index %d sub %s i %d file %s\n", *(indexes), sub_word, i, file);
			if (!right_wildcard(file, sub_word, &i))
				return (false);
			printf("\nafter index %d sub %s i %d file %s\n", *indexes, sub_word, i, file);
		}
		//printf("pattern %s start %d end %d\n", to_be_extended, *indexes, *(indexes + 1) - *(indexes) - 1);
		// if (*(indexes) == 0 && (*(to_be_extended + *indexes + 1) != '\0'))
		// {
		// 	if (!start(file, sub_word, &i))
		// 		return (false);
		// 	printf("start index %d sub %s i %d\n", *indexes, sub_word, i);
		// }
		// else
		// {
		// 	if (!inbetween(file, sub_word, &i))
		// 		return (false);
		// 	printf("after index %d sub %s i %d\n", *indexes, sub_word, i);
		// }
		// else if (*(indexes + 1) == -1 && *(to_be_extended + *indexes - 1) == '\0')
		// {
		// 	printf("index %d %s\n", *indexes, to_be_extended + *indexes - 1);
		// 	if (!end(file, sub_word))
		// 		return (false);
		// }
		// else if (!move_file_index(file, sub_word, &i))
		// 	return (false);
		prev_index = *indexes;
		indexes++;
		//free(sub_word);
	}
	return (true);
}

static char	**match_files(t_data *data, char *to_be_extended, int *indexes)
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
			printf("filename, %s\n", direntStruct->d_name);
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
		int i = 0;
		// while(data->argv[i])
		// 	printf("\t%s %d\t", data->argv[i++], argv_i);
		// printf("\n");
		files++;
	}
}

char	*get_all_names(t_data *data)
{
	int	i;
	int	j;
	int	*indexes;
	char	*to_be_extended;
	char	**files;

	i = 0;
	while (data->argv[i])
	{
		data->argv[i] = skip_d(data, data->argv[i], '*');
		indexes = get_indexes(data->argv[i]);
		j = 0;
		while (indexes[j] != -1)
			printf("index %d\t", indexes[j++]);
		printf("\n");
		if (*indexes != -1)
		{
			to_be_extended = ft_strdup(data->argv[i]);
			files = match_files(data, to_be_extended, indexes);
			//extend_wildcards(data, to_be_extended, files, i);
			free(files);
		}
		free(indexes);
		i++;
	}
	return NULL;
}