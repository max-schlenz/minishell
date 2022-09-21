/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/20 14:04:11 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_vars(t_data *data)
{
	data->prompt = ft_strdup("\x01\033[34;1m\x02mini\x01\033[36;1m\x02shell\x01\033[0;1m\x02 # \x01\033[0m\x02");
	data->flags->debug = true;
	data->flags->macos = false;
	data->flags->pipe = false;
	data->flags->error = false;
	data->file_name = NULL;
	data->file_name2 = NULL;
	data->heredoc_delim = NULL;
	data->counter_env = 0;
	data->exit_status = 0;
	data->last_cmd = NULL;
	data->flags->exit_code_of = false;
	data->flags->noenv = false;
	data->path = NULL;
	if (data->flags->debug)
	{
		data->debug = fopen("debug", "w");
		fclose(data->debug);
	}
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
		ms_exit(1, 1);
	data->pipes = ft_calloc(1, sizeof(t_pipes));
	if (!data->pipes)
		ms_exit(1, 1);
	data->flags = ft_calloc(1, sizeof(t_flags));
	if (!data->flags)
		ms_exit(1, 1);
	return (data);
}
