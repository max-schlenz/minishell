/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 17:26:31 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/17 17:50:03 by tdehne           ###   ########.fr       */
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
			new_argv[j] = ft_strdup(filename);
			j++;
			continue;
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
	tmp = NULL;
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
static void	extend_wildcards(t_data *data, char *to_be_extended, int to_be_extended_i, int argv_i)
{
	char	**files = (char **)ft_calloc(sizeof(char *), 4);
	files[0] = "lol";
	files[1] = "lall";
	files[2] = "lball";
	files[3] = NULL;
	while (*files)
	{
		if (!ft_strncmp(data->argv[argv_i], to_be_extended, ft_strlen(to_be_extended) + 1))
		{
			data->argv = realloc_argv(data, argv_i, *files, 1);
		}
		else
		{
			data->argv = realloc_argv(data, argv_i, *files, 0);
		}
		files++;
	}
}

char	*get_all_names(t_data *data)
{
	int	i;
	int	j;
	int	*indexes;
	char	*to_be_extended;

	i = 0;
	while (data->argv[i])
	{
		data->argv[i] = skip_d(data, data->argv[i], '*');
		indexes = get_indexes(data->argv[i]);
		j = 0;
		to_be_extended = data->argv[i];
		while (indexes[j])
		{
			extend_wildcards(data, to_be_extended, j, i);
			j++;
		}
		i++;
		if (indexes)
			free(indexes);
	}
	i = 0;
	while(data->argv[i])
		printf("a%sa\n", data->argv[i++]);
	exit(0);
	return NULL;
}