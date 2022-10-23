/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:21:43 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/23 10:48:46 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*get_var_content(t_data *data, char *var)
{
	int		i;
	int		len_var;

	i = 0;
	if (*var == '~')
		var = "$HOME";
	var++;
	len_var = ft_strlen(var);
	if (!len_var)
		return (NULL);
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], var, len_var))
		{
			if (ft_strlen(data->envp[i]) < len_var + 1)
				return (ft_strdup(""));
			return (ft_strdup(data->envp[i] + len_var + 1));
		}
		i++;
	}
	return (ft_strdup(""));
}
