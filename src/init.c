/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/30 15:23:31 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_msdata	*allocate_mem(void)
{
	t_msdata	*data;

	data = ft_calloc(1, sizeof(t_msdata));
	if (!data)
		exit (EXIT_FAILURE);
	data->args = ft_calloc(1, sizeof(char));
	if (!data->args)
		exit (EXIT_FAILURE);
	else
		return (data);
}