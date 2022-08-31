/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 17:00:13 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_msdata	*allocate_mem(void)
{
	t_msdata	*data;

	data = ft_calloc(1, sizeof(t_msdata));
	if (!data)
		ft_exit(1);
	data->args = ft_calloc(1, sizeof(char));
	if (!data->args)
		ft_exit(1);
	return (data);
}
