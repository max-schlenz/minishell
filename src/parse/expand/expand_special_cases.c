/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special_cases.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:04:39 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/28 12:44:29 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	expand_vars_exit(t_data *data)
{
	data->expvar.i_char++;
	data->expvar.vcontent = ft_itoa(data->exit_status);
}

void	expand_vars_shell(t_data *data)
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

void	expand_vars_not_exist(t_data *data)
{
	data->expvar.str_after_v
		= ft_strdup(data->argv[data->expvar.i_arg]
			+ data->expvar.i_char
			+ ft_strlen(data->expvar.vname));
	free(data->argv[data->expvar.i_arg]);
	data->argv[data->expvar.i_arg]
		= ft_strjoin_dup(data->expvar.str_before_v, data->expvar.str_after_v);
	free_null(3, data->expvar.str_before_v,
		data->expvar.str_after_v, data->expvar.vname);
}
