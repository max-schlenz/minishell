/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualize_btree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 16:14:06 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/02 16:24:41 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	visualize(t_btree *head)
{
	printf("-----------------\n");
	while (head)
	{
		printf ("node[val %s]\n", head->value);
		if (head->left)
			printf("left[val %s]\n", head->left->value);
		if (head->right)
		printf("right[val %s]\n", head->right->value);
		printf("-----------------\n");
		head = head->left;
	}
}