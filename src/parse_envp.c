/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 17:36:31 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_path(t_data *data)
{
	int		i;
	char	**paths;

	i = 0;
	while (ft_strncmp(data->env->var, "PATH", 5) && ++i <= data->counter_envv)
		data->env = data->env->next;
	paths = ft_split(data->env->content, ':');
	while (*paths)
		printf("%s\n", *paths++);
}

void	parse_envp(t_data *data, char **envp)
{
	char	**elem_envv;

	while (envp[data->counter_envv])
	{
		elem_envv = ft_split(envp[data->counter_envv++], '=');
		ft_lstadd_back(&data->env, ft_lstnew(data, elem_envv[0], elem_envv[1]));
		free(elem_envv);
	}
	data->env->last->next = data->env;
}