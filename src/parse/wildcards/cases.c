/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 14:02:44 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/15 14:57:49 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	match_end(t_data *data, char *str, char *pattern)
{
	int	index;

	if (!*str && *pattern != '\0')
		return (false);
	index = ft_strlen(str) - ft_strlen(pattern);
	while (*pattern && *pattern != '*')
	{
		if (str[index] != *pattern)
			return (false);
		index++;
		pattern++;
	}
	data->wcmatch.i = ft_strlen(str);
	return (true);
}

bool	match_inbetween(t_data *data, char *str, char *pattern)
{
	int	i;
	int	j;

	while (str[data->wcmatch.i])
	{
		j = 0;
		i = data->wcmatch.i;
		while (pattern[j] && pattern[j] != '*')
		{
			if (str[i] != pattern[j])
				break ;
			i++;
			j++;
		}
		if (!pattern[j])
			break ;
		data->wcmatch.i++;
	}
	data->wcmatch.i += j;
	if (pattern[j])
		return (false);
	return (true);
}

bool	match_start(t_data *data, char *str, char *pattern)
{
	while (*pattern)
	{
		if (str[data->wcmatch.i] != *pattern)
			return (false);
		data->wcmatch.i++;
		pattern++;
	}
	return (true);
}
