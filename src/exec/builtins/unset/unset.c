/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:05 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 13:18:34 by mschlenz         ###   ########.fr       */
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

static bool	builtin_unset_parse(t_data *data, int index_arg)
{
	int	i;

	if (data->argv[index_arg][0] == '-')
	{
		exec_error(data, 4, data->argv[index_arg], 2);
		return (false);
	}
	i = 0;
	if (!isidentifier(data->argv[index_arg][0])
		|| ft_strchr(data->argv[index_arg], '='))
	{
		exec_error(data, 5, data->argv[index_arg], 1);
		return (false);
	}
	while (data->argv[index_arg][i])
	{
		if (!export_check_str(data->argv[index_arg]))
		{
			exec_error(data, 5, data->argv[index_arg], 1);
			return (false);
		}
		i++;
	}
	return (true);
}

static void	builtin_unset_realloc(t_data *data, int *i)
{
	unset_realloc(data, i);
	parse_path(data);
	sort_array(data);
	(*i) = 0;
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
		if (!builtin_unset_parse(data, index_arg))
			return (true);
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
		builtin_unset_realloc(data, &i);
		index_arg++;
	}
	return (data->exit_status = 0, true);
}
