/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/10 10:42:54 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_vars(t_data *data, char **argv)
{
	data->prompt = ft_strdup("\033[34;1mmini\033[36;1mshell\033[0;1m # \033[0m");
	data->flags->pipe = false;
	data->flags->error = false;
	data->counter_env = 0;
	data->exit_status = 0;
}

void	open_pipes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->counter_pipes)
		pipe(data->pipes->pipefd[i++]);
}

t_data	*allocate_mem()
{
	t_data *data;
	
	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		ft_exit(1);
	data->pipes = ft_calloc(1, sizeof(t_pipes));
	data->flags = ft_calloc(1, sizeof(t_flags));
	if (!data)
		ft_exit(1);
	return (data);
}
