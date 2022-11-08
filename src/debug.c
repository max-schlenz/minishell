/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 11:38:28 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/08 12:57:54 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	dbg(t_data *data)
{
	int	i;

	i = 0;
	data->debug = fopen(".debug", "a+");
	fprintf(data->debug, "\n------------------\n");
	while (data->argv[i])
	{
		fprintf(data->debug, "argv[%d] = %s\n", i, data->argv[i]);
		i++;
	}
	fprintf(data->debug, "\nFLAGS:\n");
	fprintf(data->debug, "redir_out    : %d\n", data->flags->redir_out);
	fprintf(data->debug, "redir_in     : %d\n", data->flags->redir_in);
	fprintf(data->debug, "heredoc      : %d\n", data->flags->heredoc);
	fprintf(data->debug, "redir_append : %d\n", data->flags->redir_append);
	fprintf(data->debug, "pipe         : %d\n\n", data->flags->pipe);
	fprintf(data->debug, "and          : %d\n", data->flags->and);
	fprintf(data->debug, "or           : %d\n", data->flags->or);
	fprintf(data->debug, "data->fd_i            : %d\n", data->fd_i);
	fprintf(data->debug, "data->counter_pipes   : %d\n", data->counter_pipes);
	fprintf(data->debug, "data->file_name       : %s\n", data->file_name);
	fprintf(data->debug, "data->file_name2      : %s\n", data->file_name2);
	fprintf(data->debug, "data->file_name_append: %s\n", data->file_name_append);
	fprintf(data->debug, "data->exit_status (p) : %lld\n", data->exit_status);
	fclose(data->debug);
}