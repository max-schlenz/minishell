/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 11:46:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/31 16:03:30 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_mslstdelone(t_msenv *lst)
{
	if (!lst)
		return ;
	free (lst);
}

void	ft_mslstclear(t_msdata *data)
{
	t_msenv	*buf_list;

	if (!data)
		return ;
	while (data->env)
	{
		buf_list = data->env->next;
		ft_mslstdelone(data->env);
		data->env = buf_list;
	}
}

t_msenv	*ft_mslstlast(t_msenv *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_mslstadd_back(t_msenv **lst, t_msenv *new)
{
	t_msenv	*list;

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

void	ft_mslstadd_front(t_msenv **lst, t_msenv *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}
