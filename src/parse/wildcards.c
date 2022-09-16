/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 17:26:31 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/16 15:53:42 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*realloc_cmd(char *cmd, char *file_name, int insert_start)
{
	int	i = 0;
	int	j = 0;
	int	len_cmd = ft_strlen(cmd);
	int len_file_name = ft_strlen(file_name);
	char *new = (char *)ft_calloc(sizeof(char), len_cmd + file_name + 2);
	while (cmd[i])
	{
		if (j == insert_start)
		{
			while (*file_name)
				new[j++] = *file_name++;
			new[j] = ' ';
		}
		new[j] = cmd[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}

char *delete_str(char *cmd, int start, int end)
{
	char	*ret;
	int		len;
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
	indexes[j] = NULL;
	return (indexes);
}

char	*get_all_names(t_data *data)
{
	int	i;

	i = 0;
	while (data->argv[i])
	{
		if (is_wildcard(data->argv[i]))
		{
			data->argv = extend_wildcards(data, i);
		}
		i++;
	}
}