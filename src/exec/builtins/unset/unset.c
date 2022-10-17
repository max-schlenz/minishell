/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:05 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/17 21:19:55 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	unset_realloc(t_data *data, int *i)
{
	while ((*i) < data->counter_env)
	{
		free(data->envp[(*i)]);
		data->envp[(*i)] = NULL;
		if (!data->envp[(*i) + 1])
			break ;
		data->envp[(*i)] = ft_strdup(data->envp[(*i) + 1]);
		(*i)++;
	}
	if (data->envp[(*i)])
		free(data->envp[(*i)]);
	data->envp[(*i)] = NULL;
}

bool	builtin_unset(t_data *data)
{
	int	i;
	int	index_arg;
	int	len;

	i = 0;
	index_arg = 1;
	while (data->argv[index_arg])
	{
		if (data->argv[index_arg][0] == '-')
			return (builtin_error(data, data->argv[index_arg]));
		while (data->envp[i])
		{
			len = ft_strlen(data->argv[index_arg]);
			if (!ft_strncmp(data->envp[i], data->argv[index_arg], len)
				&& (!data->envp[i][len] || data->envp[i][len] == '='))
			{
				data->counter_env--;
				break ;
			}
			i++;
		}
		unset_realloc(data, &i);
		parse_path(data);
		sort_array(data);
		i = 0;
		index_arg++;
	}
	data->exit_status = 0;
	return (true);
}
