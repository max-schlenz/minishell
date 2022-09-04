/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_btree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:51:04 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/04 13:15:58 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_btree	*lstnew_btree(char *content)
{
	t_btree	*list;

	list = ft_calloc(1, sizeof(t_btree));
	if (!list)
		return (NULL);
	list->value = content;
	list->left = NULL;
	list->right = NULL;
	return (list);
}

t_btree	*lstlast_left(t_btree *lst)
{
	if (!lst)
		return (0);
	while (lst->left)
		lst = lst->left;
	return (lst);
}

t_btree	*lstlast_right(t_btree *lst)
{
	if (!lst)
		return (0);
	while (lst->right)
		lst = lst->right;
	return (lst);
}


void	lstadd_right(t_btree **lst, t_btree *new)
{
	t_btree	*list;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	list = lstlast_right(*lst);
	if (list)
		list->right = new;
}

void	lstadd_left(t_btree **lst, t_btree *new)
{
	t_btree	*list;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	list = lstlast_left(*lst);
	if (list)
		list->left = new;
}

void	lstrm_node(t_btree **head, t_btree *to_be_removed)
{
	t_btree	*tmp;
	t_btree	*tmp_left_left;

	tmp = *head;
	if (!head || !*head || !to_be_removed)
		return ;
	if (*head == to_be_removed)
	{
		free((*head)->right);
		(*head) = (*head)->left;
		free(tmp);
		return ;
	}
	while (tmp->left != to_be_removed)
		tmp = tmp->left;
	if (tmp->left)
	{
		if (tmp->left->right)
			free(tmp->left->right);
		tmp_left_left = tmp->left->left;
		tmp->left->left = NULL;
		tmp->left = tmp_left_left;
	}
}

void	lst_clear_btree(t_data *data)
{
	t_btree	*buf_list;

	if (!data->btree)
		return ;
	while (*data->btree)
	{
		buf_list = (*data->btree)->left;
		free (*data->btree);
		(*data->btree) = buf_list;
	}
	data->counter_btree = 0;
}
