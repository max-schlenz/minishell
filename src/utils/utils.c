/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/12 14:39:44 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	strdiff(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i]&& s2[i])
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
	i++;
	}
	if ((!s1[i] && s2[i] || !s2[i] && s1[i]))
		return (1);
	return (0);
}

void	history(t_data *data)
{
	if (strdiff(data->cmd, data->last_cmd))
	{
		add_history(data->cmd);
		write(data->mscfg, "HISTORY=", 8);
		write(data->mscfg, data->cmd, ft_strlen(data->cmd));
		write(data->mscfg, "\n", 1);
	}
	data->last_cmd = ft_strdup(data->cmd);
}
