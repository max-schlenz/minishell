/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 15:23:18 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/08 14:31:02 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_vars(t_data *data, char **argv)
{
	if (argv[1])
	{
		if (!ft_strncmp(argv[1], "white", 6) || !ft_strncmp(argv[1], "w", 2))
			data->prompt = "minishell > ";
		else if (!ft_strncmp(argv[1], "red", 4) || !ft_strncmp(argv[1], "r", 2))
			data->prompt = "\033[0;31mminishell\033[0m > ";
		else if (!ft_strncmp(argv[1], "green", 6) || !ft_strncmp(argv[1], "g", 2))
			data->prompt = "\033[0;32mminishell\033[0m > ";
		else if (!ft_strncmp(argv[1], "yellow", 7) || !ft_strncmp(argv[1], "y", 2))
			data->prompt = "\033[0;33mminishell\033[0m > ";
		else if (!ft_strncmp(argv[1], "blue", 5) || !ft_strncmp(argv[1], "b", 2))
			data->prompt = "\033[0;34mminishell\033[0m > ";
		else if (!ft_strncmp(argv[1], "magenta", 8) || !ft_strncmp(argv[1], "m", 2))
			data->prompt = "\033[0;35mminishell\033[0m > ";
		else if (!ft_strncmp(argv[1], "cyan", 5) || !ft_strncmp(argv[1], "c", 2))
			data->prompt = "\033[0;36mminishell\033[0m > ";
	}
	else
		data->prompt = "\033[34;1mmini\033[36;1mshell\033[0m > ";
	data->flags.pipe = false;
	data->flags.error = false;
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
