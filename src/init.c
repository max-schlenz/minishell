/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/01 13:37:01 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_data	*allocate_mem(void)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		ft_exit(1);
	// data->args = ft_calloc(1, sizeof(char));
	// if (!data->args)
	// 	ft_exit(1);
	data->counter_envv = 0;
	return (data);
}
