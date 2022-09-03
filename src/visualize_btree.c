/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualize_btree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 16:14:06 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/03 13:33:39 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	visualize(t_btree *head)
{
	printf("-----------------\n");
	// while (head)
	// {
		printf ("%s\n", head->value);
		if (head->left)
			printf("%s\n", head->left->value);
		if (head->right)
		printf("%s\n", head->right->value);
		printf("-----------------\n");
		// head = head->left;
	// }
}