/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/06 11:51:02 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_vars(t_data *data)
{
	data->flag_pipe = false;
	data->flag_error = false;
	data->counter_env = 0;
}

t_data	*allocate_mem()
{
	t_data *data;
	
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		ft_exit(1);
	data->pipes = ft_calloc(1, sizeof(t_pipes));
	return (data);
}
