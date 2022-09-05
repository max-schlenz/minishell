/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_envp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/05 14:33:12 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_path(t_data *data)
{
	int		i;

	i = 0;
	if (data->path)
	{
		while (*data->path)
			free (*data->path++);
	}
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	data->path = ft_split(data->envp[i], ':');
}

void	parse_envp(t_data *data, char **envp)
{
	int	i;

	while (envp[data->counter_env])
		data->counter_env++;
	data->envp = ft_calloc(data->counter_env + 1, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
}
