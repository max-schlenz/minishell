/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:56:25 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/03 18:57:19 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_mslstnew(t_data *data, char *var, char *content)
{
	t_env	*list;

	list = malloc(sizeof(t_env));
	if (!list)
		return (NULL);
	list->var = var;
	list->content = content;
	list->next = NULL;
	list->last = list;
	return (list);
}

void	ft_mslstclear(t_data *data)
{
	t_env	*buf_list;
	int		i = 0;

	if (!data)
		return ;
	while (i++ < data->counter_envv)
	{
		buf_list = data->env->next;
		ft_mslstdelone(data->env);
		data->env = buf_list;
	}
	data->counter_envv = 0;
	data->env = NULL;
}