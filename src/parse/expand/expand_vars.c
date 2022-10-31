/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 09:46:57 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/31 14:07:05 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	expand_vars_join(t_data *data)
{
	data->var.pre_and_value = ft_strjoin(data->var.pre, data->var.value);
	data->var.rest = ft_strdup(data->argv[data->var.i_arg]
			+ data->var.i_char + ft_strlen(data->var.name));
	free_null (1, &data->argv[data->var.i_arg]);
	if (ft_strlen(data->var.pre_and_value) > 0 || ft_strlen(data->var.rest) > 0)
		data->argv[data->var.i_arg]
			= ft_strjoin(data->var.pre_and_value, data->var.rest);
	else
		data->argv[data->var.i_arg] = ft_strdup("");
	free_null(3,
		&data->var.pre_and_value,
		&data->var.rest,
		&data->var.value);
}

static bool	expand(t_data *data)
{
	int		i_arg;
	size_t	i_char;

	i_arg = data->var.i_arg;
	i_char = data->var.i_char;
	data->var.pre = ft_substr(data->argv[i_arg], 0, i_char);
	data->var.name = ft_substr(data->argv[i_arg], i_char,
			strlen_path(data->argv[i_arg] + i_char));
	if (v_ex(data))
		return (true);
	if (data->argv[i_arg][i_char] && data->argv[i_arg][i_char + 1]
		&& !ft_strncmp(data->argv[i_arg] + i_char, "$?", 2))
		expand_vars_exit(data);
	else if (data->argv[i_arg][i_char + 1]
		&& isnumeric(data->argv[i_arg][i_char + 1]))
		expand_vars_shell(data);
	else
		data->var.value = expand_get_var_content(data, data->var.name);
	if (data->var.value)
		expand_vars_join(data);
	free_null(2, &data->var.pre, &data->var.name);
	if (i_char > ft_strlen(data->argv[i_arg]))
		return (false);
	return (true);
}

static bool	expand_vars_or_no(t_data *data)
{
	int		i;
	char	*arg;

	i = data->var.i_char;
	arg = data->argv[data->var.i_arg];
	if ((!data->flags->noenv
			&& arg[0] == '~'
			&& (!ft_isalnum(arg[1])
				&& !data->var.f_dquote && !data->var.f_squote))
		|| (arg[i] == '$'
			&& (arg[i + 1])
			&& (arg[i + 1] != ' ')
			&& (arg[i + 1] != '(')
			&& (arg[i + 1] != '=')
			&& (arg[i + 1] != '"')
			&& (!data->var.f_squote)))
		return (true);
	return (false);
}

static bool	expand_vars_handle_arg(t_data *data)
{
	size_t	i;
	char	*arg;

	i = data->var.i_char;
	arg = data->argv[data->var.i_arg];
	if (arg[i] == '\"')
		data->var.f_dquote = !data->var.f_dquote;
	if (arg[i] == '\'' && !data->var.f_dquote)
		data->var.f_squote = !data->var.f_squote;
	if (arg[i] == '\\')
	{
		if (arg[i + 1] && arg[i + 2])
			return (data->var.i_char += 2, true);
		return (false);
	}
	if (expand_vars_or_no(data))
		return (expand(data));
	if (arg[i] && arg[i + 1])
		return (data->var.i_char++, true);
	return (false);
}

bool	expand_vars(t_data *data)
{
	expand_vars_init(data, true);
	while (data->argv[data->var.i_arg])
	{
		expand_vars_reset_flags(data);
		while (data->argv[data->var.i_arg][data->var.i_char])
		{
			if (expand_vars_handle_arg(data))
				continue ;
			break ;
		}
		if (!data->argv[data->var.i_arg])
			break ;
		expand_vars_rm_mod(data);
		expand_vars_init(data, false);
	}
	return (true);
}
