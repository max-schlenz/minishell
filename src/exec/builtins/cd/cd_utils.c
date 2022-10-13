/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 09:37:25 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/05 12:25:01 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	cd_find_pwd(t_data *data)
{
	size_t	i;

	i = 0;
	while (data->envp[i] && strcmp_alnum(data->envp[i], "PWD"))
		i++;
}

void	cd_root(t_data *data)
{
	chdir("/root");
	data->cd.new_pwd_tmp = ft_strdup("PWD=/root");
}
