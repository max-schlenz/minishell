/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/07 14:32:42 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_vars(t_data *data)
{
	data->flag_pipe = false;
	data->flag_error = false;
	data->counter_env = 0;
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
	if (!data)
		ft_exit(1);
	return (data);
}
