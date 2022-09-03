/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualize_btree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 16:14:06 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/03 14:09:08 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	visualize(t_btree *head)
{
	// while (head)
	// {
		printf ("%s\n", head->value);
		if (head->left)
			printf("%s\n", head->left->value);
		if (head->right)
		printf("%s\n", head->right->value);
		// head = head->left;
	// }
}