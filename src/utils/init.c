/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/12/09 21:13:55 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	initializes/resets variables
void	init_vars(t_data *data)
{
	data->hdoc.quote = false;
	data->hdoc.hd = false;
	data->flags->macos = false;
	data->flags->pipe = false;
	data->flags->error = false;
	data->flags->reset = false;
	data->file_name = NULL;
	data->file_name2 = NULL;
	data->file_name_append = NULL;
	data->counter_env = 0;
	data->exit_status = 0;
	data->exit_code = 0;
	data->heredoc_index = 0;
	data->last_cmd = NULL;
	data->flags->exit_code_of = false;
	data->flags->noenv = false;
	data->path = NULL;
	data->cfg = NULL;
	data->exit.max = 922337203685477580;
	heredoc_init(data);
}

//	allocates memory for data struct
t_data	*allocate_mem(void)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		ms_exit(E_MEM, 1);
	data->pipes = ft_calloc(1, sizeof(t_pipes));
	if (!data->pipes)
		ms_exit(E_MEM, 1);
	data->flags = ft_calloc(1, sizeof(t_flags));
	if (!data->flags)
		ms_exit(E_MEM, 1);
	return (data);
}
