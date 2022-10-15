/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 17:26:31 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/15 14:38:45 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ext_wildcards(t_data *data, char *to_ext, char **files, int argv_i)
{
	while (*files)
	{
		if (!ft_strncmp(data->argv[argv_i], to_ext, ft_strlen(to_ext) + 1))
			data->argv = realloc_argv(data, argv_i, *files, 1);
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
			files = match_files(data, to_be_extended, indexes);
			ext_wildcards(data, to_be_extended, files, i);
			free_array(files);
			free(files);
			free(to_be_extended);
		}
		free(indexes);
		i++;
	}
}
