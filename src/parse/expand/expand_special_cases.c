/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special_cases.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:04:39 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/31 14:12:56 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	expand_vars_exit(t_data *data)
{
	data->var.i_char++;
	data->var.value = ft_itoa(data->exit_status);
}

void	expand_vars_shell(t_data *data)
{
	int		i;
	char	*arg;

	i = ++data->var.i_char;
	arg = data->argv[data->var.i_arg];
	data->var.name = realloc_ptr(data->var.name,
			ft_substr(arg, i, 1), true);
	if (arg[i] == '0')
		data->var.value = ft_strdup("minishell");
	else
		data->var.value = ft_strdup("");
}

static void	expand_realloc_argv(t_data *data, int i)
{
	data->argc--;
	data->var.i_arg--;
	data->var.i_char = 0;
	while (data->argv[i + 1])
	{
		data->argv[i] = data->argv[i + 1];
		i++;
	}
	data->argv[i] = NULL;
	free(data->argv[data->argc + 1]);
}

void	expand_vars_not_exist(t_data *data)
{
	int		i;
	size_t	i_char;
	char	*arg;

	i = data->var.i_arg;
	i_char = data->var.i_char;
	arg = data->argv[i];
	data->var.rest
		= ft_strdup(arg + i_char + ft_strlen(data->var.name));
	if (!*data->var.rest && !*data->var.pre)
		expand_realloc_argv(data, i);
	else
	{
		free_null(1, &data->argv[i]);
		data->argv[i] = ft_strjoin_dup(data->var.pre, data->var.rest);
	}
	free_null(3, &data->var.pre,
		&data->var.rest,
		&data->var.name);
}
