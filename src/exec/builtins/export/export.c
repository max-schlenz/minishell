/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 11:20:27 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	export_print(t_data *data)
{
	int		i;
	int		len_name;
	int		len_content;
	char	*str_name;
	char	*str_content;

	builtin_fork(data, false);
	if (data->pid == 0)
	{
		redirs_pipes(data);
		i = 0;
		while (data->envp[i])
		{
			len_name = strlen_var(data->envp[i]);
			len_content = ft_strlen(data->envp[i]) - len_name;
			str_name = ft_substr(data->envp[i], 0, len_name);
			str_content = ft_substr(data->envp[i], len_name + 1, len_content);
			printf("declare -x %s=\"%s\"\n", str_name, str_content);
			free (str_name);
			free (str_content);
			i++;
		}
		exit(0);
	}
	builtin_fork(data, true);
}

static void	export_set_existing(t_data *data, char *setv)
{
	data->envp[data->export.index_envp]
		= str_realloc(data->envp[data->export.index_envp], setv, 0);
	data->export.set = true;
}

static bool	export_setv(t_data *data, char *setv)
{
	data->export.index_envp = 0;
	data->export.len = strlen_var(setv);
	if (setv[data->export.len] == '=' && data->export.len < ft_strlen(setv) - 1)
	{
		while (data->envp[data->export.index_envp])
		{
			if (!ft_strncmp(data->envp[data->export.index_envp],
					setv, data->export.len + 1))
				export_set_existing(data, setv);
			data->export.index_envp++;
		}
		if (!data->export.set && !data->envp[data->export.index_envp])
			realloc_envp(data, setv, 1);
		sort_array(data);
		parse_path(data);
	}
	data->exit_status = 0;
	if (data->export.free_set)
		free (setv);
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}

static bool	export_var(t_data *data, char *setv)
{
	if (setv && setv[0] == '-')
		return (export_err_op(data, setv));
	if (setv && !export_check_str(setv))
		return (export_err_con(data, setv));
	if (setv && isidentifier(setv[0]))
		return (export_setv(data, setv));
	else if (setv)
		return (export_err_inv(data, setv));
}

bool	builtin_export(t_data *data, char *setv)
{
	data->export.index_arg = 1;
	data->export.free_set = false;
	data->export.set = false;
	while (data->argc > 0 || setv)
	{
		if (!setv)
		{
			data->export.free_set = true;
			if (data->argv[data->export.index_arg])
				setv = ft_strdup(data->argv[data->export.index_arg]);
			else
				break ;
		}
		if (setv)
		{
			if (!export_var(data, setv))
				continue ;
			return (true);
		}
	}
	if (!setv && !data->argc)
		export_print(data);
	return (true);
}
