/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_envp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/04 13:24:10 by mschlenz         ###   ########.fr       */
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
	int		i;

	data->envp = ft_calloc(data->counter_envv + 1, sizeof(char *));
	i = 0;
	while (i < data->counter_envv)
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	// data->envp[i] = NULL;
}
