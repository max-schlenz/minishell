/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_null.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:15:01 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/13 10:17:30 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	free_null(int index, ...)
{
	va_list	args;
	void	**tmp;

	va_start(args, index);
	while (index)
	{
		tmp = va_arg(args, void **);
		if (*tmp)
			free (*tmp);
		*tmp = NULL;
		index--;
	}	
	va_end(args);
}
