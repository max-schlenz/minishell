/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special_cases.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:04:39 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 13:58:30 by mschlenz         ###   ########.fr       */
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

static char	**expand_realloc_argv(t_data *data, int i)
{
	char	**ret;
	int		j;
	int		k;

	j = 0;
	k = 0;
	ret = (char **)ft_calloc(data->argc + 1, sizeof(char *));
	if (!ret)
		cleanup(data, E_MEM);
	data->argc--;
	data->var.i_arg--;
	data->var.i_char = 0;
	while (data->argv[j])
	{
		if (i != j)
			ret[k++] = ft_strdup(data->argv[j]);
		free(data->argv[j++]);
	}
	ret[k] = NULL;
	free_null(1, &data->argv);
	return (ret);
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
		data->argv = expand_realloc_argv(data, i);
	else
	{
		free_null(1, &data->argv[i]);
		data->argv[i] = ft_strjoin_dup(data->var.pre, data->var.rest);
	}
	free_null(3, &data->var.pre,
		&data->var.rest,
		&data->var.name);
}
