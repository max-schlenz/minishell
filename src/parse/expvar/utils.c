/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:01:41 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/13 13:30:48 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	expand_vars_init(t_data *data, bool flag)
{
	if (flag)
		data->expvar.i_arg = 0;
	else
		data->expvar.i_arg++;
	data->expvar.i_char = 0;
	data->expvar.vname = NULL;
}

void	expand_vars_weird_special_case(t_data *data, bool flag)
{
	free(data->argv[data->expvar.i_arg]);
	if (flag)
		data->argv[data->expvar.i_arg] = ft_strdup("");
	else
		data->argv[data->expvar.i_arg] = NULL;
}

bool	v_ex(t_data *data)
{
	if (!check_var_exists(data, data->expvar.vname)
		&& data->argv[data->expvar.i_arg][data->expvar.i_char]
		&& data->argv[data->expvar.i_arg][data->expvar.i_char + 1]
		&& ft_strncmp(data->argv[data->expvar.i_arg]
		+ data->expvar.i_char, "$?", 2))
	{
		expand_vars_ne(data);
		return (true);
	}
	return (false);
}

void	expand_vars_rm_mod(t_data *data)
{
	remove_quotes(data, data->expvar.i_arg);
	remove_backslashes(data, data->expvar.i_arg);
}

void	expand_vars_reset_flags(t_data *data)
{
	data->expvar.f_dquote = false;
	data->expvar.f_squote = false;
}
