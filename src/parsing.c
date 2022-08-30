/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 14:59:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/30 15:16:35 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*chk_escvar(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
			return ((char *)s + i);
		i++;
	}
	if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
		return ((char *)s + i);
	return (0);
}

size_t	strlen_path(const char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '/')
		i++;
	return (i);
}