/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/20 17:40:11 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	init_dbg(t_data *data)
{
	data->debug = fopen(DBG, "w");
	fclose(data->debug);
}

void	init_vars(t_data *data)
{
	data->hdoc.quote = false;
	data->flags->rndcl = false;
	data->hdoc.andor = false;
	data->hdoc.hd = false;
	data->flags->debug = true;
	data->flags->macos = true;
	data->flags->pipe = false;
	data->flags->error = false;
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
	data->debug = NULL;
	data->exit.max = 922337203685477580;
	init_hd(data);
	if (data->flags->debug)
		init_dbg(data);
}

void	open_pipes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->counter_pipes)
		pipe(data->pipes->pipefd[i++]);
}

t_data	*allocate_mem(void)
{
	t_data	*data;

	data = ft_calloc(1, sizeof(t_data));
	if (!data)
		ms_exit(1, 1);
	data->pipes = ft_calloc(1, sizeof(t_pipes));
	if (!data->pipes)
		ms_exit(1, 1);
	data->flags = ft_calloc(1, sizeof(t_flags));
	if (!data->flags)
		ms_exit(1, 1);
	return (data);
}
