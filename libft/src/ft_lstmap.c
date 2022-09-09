/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 10:53:12 by mschlenz          #+#    #+#             */
/*   Updated: 2022/05/27 15:14:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*n_list;
	t_list	*n_element;

	n_list = NULL;
	if (!lst || !f)
		return (NULL);
	while (lst)
	{
		n_element = ft_lstnew(f(lst->content));
		if (!n_element && del)
		{
			ft_lstclear(&n_list, del);
			return (NULL);
		}
		if (!n_list)
			n_list = n_element;
		else
			ft_lstadd_back(&n_list, n_element);
		lst = lst->next;
	}
	return (n_list);
}
