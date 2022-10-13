/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:31 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/13 09:26:07 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	export_check_str(char *str)
{
	int		i;

	i = 0;
	while (str[i] 
		&& ((str[i] >= 65 && str[i] <= 90)
			|| (str[i] >= 97 && str[i] <= 122)
			|| (str[i] >= 48 && str[i] <= 57)
			|| (str[i] == '_' || str[i] == '+')))
		i++;
	if (str[i] && str[i] != '=')
		return (false);
	i -= 2;
	if (i > 1)
	{
		while (str[i] && str[i] != '+')
		{
			i--;
			if (i < 0)
				break ;
		}
		if (i >= 0)
			return (false);
	}
	return (true);
}

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

static bool	export_err_op(t_data *data, char *setv)
{
	data->export.err = strjoin_nl
		("Error: export: option not supported: ", setv);
	write(2, data->export.err, ft_strlen(data->export.err));
	free (data->export.err);
	data->exit_status = 2;
	if (data->export.free_set)
	{
		free (setv);
		setv = NULL;
	}
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}

static bool	export_err_con(t_data *data, char *setv)
{
	data->export.err = strjoin_nl
		("Error: export: not valid in this context: ", setv);
	write(2, data->export.err, ft_strlen(data->export.err));
	free (data->export.err);
	if (data->export.free_set)
	{
		free (setv);
		setv = NULL;
	}
	data->exit_status = 1;
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}

static bool	export_err_inv(t_data *data, char *setv)
{
	data->exit_status = 1;
	data->export.err = strjoin_nl
		("Error: export: not a valid identifier: ", setv);
	write(2, data->export.err, ft_strlen(data->export.err));
	free (data->export.err);
	if (data->export.free_set)
	{
		free (setv);
		setv = NULL;
	}
	return (true);
}

static bool export_setv(t_data *data, char *setv)
{
	data->export.index_envp = 0;
	data->export.len = strlen_var(setv);
	if (setv[data->export.len] == '=')
	{
		if (data->export.len < ft_strlen(setv) - 1)
		{
			while (data->envp[data->export.index_envp])
			{
				if (!ft_strncmp(data->envp[data->export.index_envp],
						setv, data->export.len + 1))
				{
					free(data->envp[data->export.index_envp]);
					data->envp[data->export.index_envp] = ft_strdup(setv);
				}
				data->export.index_envp++;
			}
			if (!data->export.len && !data->envp[data->export.index_envp])
				realloc_envp(data, setv, 1);
			sort_array(data);
			parse_path(data);
		}
	}
	data->exit_status = 0;
	if (data->export.free_set)
		free (setv);
	if (data->export.index_arg++ < data->argc)
		return (false);
	return (true);
}

bool	builtin_export(t_data *data, char *setv)
{
	data->export.index_arg = 1;
	data->export.free_set = false;
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
		if (setv && setv[0] == '-')
		{
			if (!export_err_op(data, setv))
				continue ;
			return (true);
		}
		if (setv && !export_check_str(setv))
		{
			if (!export_err_con(data, setv))
				continue ;
			return (true);
		}
		if (setv && isidentifier(setv[0]))
		{
			if (!export_setv(data, setv))
				continue ;
			return (true);
		}
		else if (setv)
			return (export_err_inv(data, setv));
	}
		if (!setv && !data->argc)
			export_print(data);
		return (true);
}
