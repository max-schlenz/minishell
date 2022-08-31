/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 11:46:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 18:04:21 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_mslstdelone(t_env *lst)
{
	if (!lst)
		return ;
	free (lst);
}

void	ft_mslstclear(t_data *data)
{
	t_env	*buf_list;

	if (!data)
		return ;
	while (data->env)
	{
		buf_list = data->env->next;
		ft_mslstdelone(data->env);
		data->env = buf_list;
	}
}

t_env	*ft_mslstlast(t_env *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_mslstadd_back(t_env **lst, t_env *new)
{
	t_env	*list;

	list = NULL;
	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	if ((*lst) != NULL)
		list = (*lst)->last;
	if (list)
		list->next = new;
	(*lst)->last = new;
}

void	ft_mslstadd_front(t_env **lst, t_env *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}
