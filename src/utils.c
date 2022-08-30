/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/30 15:20:00 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exit(int flag)
{
	// system("leaks minishell");
	if (flag == 0)
		exit(EXIT_SUCCESS);
	else if (flag == 1)
		printf("%s", E_MEM);
	else if (flag == 2)
		printf("%s", E_FORK);
	exit (EXIT_FAILURE);
}

void	cleanup(t_msdata *data, int flag)
{
	// if (data->env)
	// 	ft_mslstclear(data);
	if (data->args)
		free(data->args);
	if (data->path)
		free (data->path);
	if (data->cmd)
		free(data->cmd);
	if (data)
		free(data);
	ft_exit(0);
}