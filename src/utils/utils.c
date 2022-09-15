/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 11:23:55 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/15 13:18:50 by mschlenz         ###   ########.fr       */
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
	if ((!s1[i] && s2[i]) || (!s2[i] && s1[i]))
		return (1);
	return (0);
}

void	history(t_data *data, char *cmd)
{
	if (strdiff(cmd, data->last_cmd))
	{
		add_history(cmd);
		write(data->mscfg, "HISTORY=", 8);
		write(data->mscfg, cmd, ft_strlen(cmd));
		write(data->mscfg, "\n", 1);
	}
	if (data->last_cmd)
		free (data->last_cmd);
	data->last_cmd = ft_strdup(cmd);
}
