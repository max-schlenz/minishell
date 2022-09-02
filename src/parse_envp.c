/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/02 19:16:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_path(t_data *data)
{
	int		i;
	t_env	*tmp_env;

	i = 0;
	tmp_env = data->env;
	if (data->path)
	{
		while (*data->path)
			free (*data->path++);
	}
	while (ft_strncmp(tmp_env->var, "PATH", 5) && ++i <= data->counter_envv)
		tmp_env = tmp_env->next;
	data->path = ft_split(tmp_env->content, ':');

}

void	parse_envp(t_data *data, char **envp)
{
	char	**elem_envv;
	t_env	*node;

	while (envp[data->counter_envv])
	{
		elem_envv = ft_split(envp[data->counter_envv++], '=');
		if (!data->env)
			data->env = ft_mslstnew(data, elem_envv[0], elem_envv[1]);
		else
		{
			node = ft_mslstnew(data, elem_envv[0], elem_envv[1]);
			ft_mslstadd_back(&data->env, node);
		}
		free(elem_envv);
	}
	data->env->last->next = data->env;
}