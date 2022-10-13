/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_cases.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:04:39 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/13 12:05:36 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	expand_vars_exit(t_data *data)
{
	data->expvar.i_char++;
	data->expvar.vcontent = ft_itoa(data->exit_status);
}

void	expand_vars_shl(t_data *data)
{
	data->expvar.i_char++;
	free (data->expvar.vname);
	data->expvar.vname
		= ft_substr(data->argv[data->expvar.i_arg], data->expvar.i_char, 1);
	if (data->argv[data->expvar.i_arg][data->expvar.i_char] == '0')
		data->expvar.vcontent = ft_strdup("minishell");
	else
		data->expvar.vcontent = ft_strdup("");
}

void	expand_vars_ne(t_data *data)
{
	data->expvar.str_after_v
		= ft_strdup(data->argv[data->expvar.i_arg]
			+ data->expvar.i_char
			+ ft_strlen(data->expvar.vname));
	free(data->argv[data->expvar.i_arg]);
	if (ft_strlen(data->expvar.str_before_v) > 0)
		data->argv[data->expvar.i_arg]
			= ft_strjoin(data->expvar.str_before_v, data->expvar.str_after_v);
	else
		data->argv[data->expvar.i_arg] = ft_strdup("");
	free_str(3, data->expvar.str_before_v,
		data->expvar.str_after_v, data->expvar.vname);
}