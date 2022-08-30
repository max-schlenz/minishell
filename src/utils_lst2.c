/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 18:06:11 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/07 10:14:47 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	ft_mslstsize(t_msenv *lst)
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

t_msenv	*ft_mslstnew(t_msdata *data, char *var, char *content)
{
	t_msenv	*list;

	list = malloc(sizeof(t_msenv));
	if (!list)
		return (NULL);
	list->var = var;
	list->content = content;
	list->next = NULL;
	list->last = list;
	return (list);
}
