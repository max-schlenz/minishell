/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:53:59 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/03 19:11:00 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

size_t	ft_mslstsize(t_env *lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst->next)
	{
		i++;
		lst = lst->next;
	}
	return (i + 1);
}

t_env	*ft_mslstlast(t_env *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_mslstdelone(t_env *lst)
{
	if (!lst)
		return ;
	free (lst);
}

void	env_lstrm(t_env **head, t_env *to_be_removed)
{
	if (!head || !*head || to_be_removed)
		return ;

}