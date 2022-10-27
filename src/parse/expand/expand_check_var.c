/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_check_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:31:50 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 17:33:28 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	expand_check_var_exists(t_data *data, char *var)
{
	int		i;
	char	*tmp;

	i = 0;
	if ((var && (var[0] == '~' || !ft_strncmp(var, "$?", 2)))
		|| (var && var[0] && var[1] && isnumeric(var[1])))
		return (true);
	tmp = ft_strjoin(var + 1, "=");
	while (data->envp[i])
	{
		if (!ft_strncmp(tmp, data->envp[i], ft_strlen(tmp)))
		{
			free(tmp);
			return (true);
		}
		i++;
	}
	free(tmp);
	return (false);
}
