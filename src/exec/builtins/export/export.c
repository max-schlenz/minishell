/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 13:31:50 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	export_print(t_data *data)
{
	int		i;
	int		len_name;
	int		len_val;
	char	*name;
	char	*val;

	i = 0;
	while (data->envp[i])
	{
		len_name = strlen_var(data->envp[i]);
		len_val = ft_strlen(data->envp[i]) - len_name;
		name = ft_substr(data->envp[i], 0, len_name);
		val = ft_substr(data->envp[i], len_name + 1, len_val);
		export_output(len_val, name, val);
		free_null (2, &name, &val);
		i++;
	}
}

static void	export_set_existing(t_data *data, char *setv)
{
	data->envp[data->export.index_envp]
		= realloc_ptr(data->envp[data->export.index_envp], setv, false);
	data->export.set = true;
}

static void	export_set_var_nval(t_data *data, char *setv)
{
	while (data->envp[data->export.index_envp])
	{
		if (!ft_strncmp(data->envp[data->export.index_envp], \
						setv, data->export.len))
			export_set_existing(data, setv);
		data->export.index_envp++;
	}
	if (!data->export.set && !data->envp[data->export.index_envp])
		realloc_envp(data, setv, 1);
	sort_array(data);
	parse_path(data);
}

void	export_setv(t_data *data, char *setv)
{
	data->export.index_envp = 0;
	data->export.len = strlen_var(setv);
	if (setv[data->export.len] == '=' && data->export.len < ft_strlen(setv) - 1)
	{
		while (data->envp[data->export.index_envp])
		{
			if (!ft_strncmp(data->envp[data->export.index_envp], \
					setv, data->export.len))
			{
				export_set_existing(data, setv);
				break ;
			}
			data->export.index_envp++;
		}
		if (!data->export.set && !data->envp[data->export.index_envp])
			realloc_envp(data, setv, 1);
		sort_array(data);
		parse_path(data);
	}
	else if (setv[data->export.len] != '=')
		export_set_var_nval(data, setv);
	data->exit_status = 0;
}

bool	builtin_export(t_data *data, char *setv)
{
	data->export.index_arg = 1;
	while (data->argc > 0 || setv)
	{
		data->export.set = false;
		data->export.free_set = false;
		if (!setv)
		{
			if (data->argv[data->export.index_arg])
			{
				data->export.free_set = true;
				setv = ft_strdup(data->argv[data->export.index_arg]);
			}
			else
				break ;
		}
		if (setv && !export_var(data, setv))
		{
			setv = NULL;
			continue ;
		}
		return (true);
	}
	if (!setv && !data->argc)
		export_print(data);
	return (true);
}
