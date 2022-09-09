/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/08 14:35:37 by tdehne           ###   ########.fr       */
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
	
	if (!array || !*array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
}

void	close_pipes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->counter_pipes)
	{
		close(data->pipes->pipefd[i][0]);
		close(data->pipes->pipefd[i++][1]);	
	}
}

void	cleanup(t_data *data, int flag)
{
	free_array(data->envp);
	free (data->envp);
	free_array(data->path);
	free (data->path);
	free_array(data->argv);
	free (data->argv);
	if (data->pipes)
		free(data->pipes);
	if (data)
		free(data);
	ft_exit(0);
	return ;
}