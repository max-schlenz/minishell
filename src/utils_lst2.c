/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 18:06:11 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/01 17:17:36 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

t_btree	*ft_mslstnew2(char *content)
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
