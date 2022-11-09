/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:01:41 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/09 17:34:58 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	expand_vars_init(t_data *data, bool flag)
{
	if (flag)
		data->var.i_arg = 0;
	else
		data->var.i_arg++;
	data->var.i_char = 0;
	data->var.name = NULL;
}

bool	expand_check_var_exists_util(t_data *data)
{
	int	i_arg;
	int	i_char;

	i_arg = data->var.i_arg;
	i_char = data->var.i_char;
	data->var.name = data->var.name;
	if (!expand_check_var_exists(data, data->var.name)
		&& data->argv[i_arg][i_char]
		&& data->argv[i_arg][i_char + 1]
		&& ft_strncmp(data->argv[i_arg] + i_char, "$?", 2))
	{
		expand_vars_not_exist(data);
		return (true);
	}
	return (false);
}

void	expand_vars_reset_flags(t_data *data)
{
	data->var.f_dquote = false;
	data->var.f_squote = false;
}
