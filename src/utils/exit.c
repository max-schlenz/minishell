/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/07 10:04:44 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exit(t_status flag)
{
	// system("leaks minishell");
	if (flag == SUCCESS)
		exit(EXIT_SUCCESS);
	else if (flag == E_MEM)
		printf("%s", E_MEM_MSG);
	else if (flag == E_FORK)
		printf("%s", E_FORK_MSG);
	exit (EXIT_FAILURE);
}

void free_array(char **array)
{
	int	i;
	
	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	cleanup(t_data *data, int flag)
{
	free_array(data->envp);
	free_array(data->path);
	free_array(data->argv);
	if (data)
		free(data);
	ft_exit(0);
	return ;
}