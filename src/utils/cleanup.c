/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/15 13:11:47 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_exit(t_status flag, int exit_status)
{
	if (flag == SUCCESS)
		exit(exit_status);
	else if (flag == E_MEM)
		printf("%s", E_MEM_MSG);
	else if (flag == E_FORK)
		printf("%s", E_FORK_MSG);
	exit (exit_status);
}

void	free_argv(t_data *data, char **array)
{
	int	i;

	if (!array || !*array)
		return ;
	i = 0;
	while (i <= data->argc)
		free(array[i++]);
}

void	free_array(char **array)
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
	while (i <= data->counter_pipes)
	{
		close(data->pipes->pipefd[i][0]);
		close(data->pipes->pipefd[i++][1]);
	}
}

void	cleanup(t_data *data, int flag)
{
	int	exit_status;

	exit_status = data->exit_status;
	free_array(data->envp);
	free(data->envp);
	free_array(data->path);
	free(data->path);
	free (data->prompt);
	free (data->last_cmd);
	free (data->flags);
	free (data->color.code[0]);
	free (data->color.code[1]);
	if (data->cmd)
		free (data->cmd);
	rl_clear_history();
	if (data->pipes)
		free(data->pipes);
	free_hd(data);
	if (data)
		free(data);
	ms_exit(flag, exit_status);
}
