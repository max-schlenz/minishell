/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 18:47:06 by mschlenz         ###   ########.fr       */
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
	while (ft_strncmp(data->env->var, "PATH", 5) && ++i <= data->counter_envv)
		data->env = data->env->next;
	data->path = ft_split(data->env->content, ':');
}

void	parse_envp(t_data *data, char **envp)
{
	char	**elem_envv;

	while (envp[data->counter_envv])
	{
		elem_envv = ft_split(envp[data->counter_envv++], '=');
		ft_mslstadd_back(&data->env, ft_mslstnew(data, elem_envv[0], elem_envv[1]));
		free(elem_envv);
	}
	data->env->last->next = data->env;
}