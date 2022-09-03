/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 11:46:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 15:25:44 by mschlenz         ###   ########.fr       */
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

t_btree	*ft_lstlast2(t_btree *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_btree	*ft_lstlast_left(t_btree *lst)
{
	if (!lst)
		return (0);
	while (lst->left)
		lst = lst->left;
	return (lst);
}

t_btree	*ft_lstlast_right(t_btree *lst)
{
	if (!lst)
		return (0);
	while (lst->right)
		lst = lst->right;
	return (lst);
}

void	ft_mslstadd_right(t_btree **lst, t_btree *new)
{
	t_btree	*list;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	list = ft_lstlast_right(*lst);
	if (list)
		list->right = new;
}

void	ft_mslstadd_left(t_btree **lst, t_btree *new)
{
	t_btree	*list;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	list = ft_lstlast_left(*lst);
	if (list)
		list->left = new;
}
