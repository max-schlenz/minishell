/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 14:00:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 14:07:58 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	len_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
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

char	**realloc_argv(t_data *data, int argv_i, char *file, bool repl)
{
	char	**new_argv;
	int		i;
	int		j;

	new_argv = (char **)ft_calloc(sizeof(char *), len_argv(data->argv) + 2);
	if (!new_argv)
		cleanup(data, E_MEM);
	i = 0;
	j = 0;
	while (data->argv[i])
	{
		new_argv[j] = ft_strdup(data->argv[i]);
		if (j == argv_i && repl)
		{
			free(new_argv[j]);
			new_argv[j] = ft_strdup(file);
		}
		else if (j == argv_i && !repl)
			new_argv[++j] = ft_strdup(file);
		free(data->argv[i]);
		i++;
		j++;
	}
	new_argv[j] = NULL;
	free(data->argv);
	return (new_argv);
}

char	*delete_str(t_data *data, char *cmd, size_t start, size_t end)
{
	char		*ret;
	size_t		len;
	size_t		i;
	int			j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len - (start - end) + 1);
	if (!ret)
		cleanup(data, E_MEM);
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

int	*get_indexes(t_data *data, char *str)
{
	int	i;
	int	j;
	int	*indexes;
	int	*tmp;

	i = 0;
	j = 0;
	indexes = (int *)ft_calloc(sizeof(int), num_wildcards(str) + 1);
	if (!indexes)
		cleanup(data, E_MEM);
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
