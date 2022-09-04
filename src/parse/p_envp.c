/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_envp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/04 13:58:59 by tdehne           ###   ########.fr       */
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
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH", 5))
		i++;
	data->path = ft_split(data->envp[i], ':');
}

void	parse_envp(t_data *data, char **envp)
{
	int		i;

	while (envp[i])
		i++;
	data->envp = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	data->envp = envp;
}
