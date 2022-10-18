/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 09:37:25 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 12:49:41 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	cd_find_pwd(t_data *data)
{
	size_t	i;

	i = 0;
	while (data->envp[i] && strcmp_alnum(data->envp[i], "PWD"))
		i++;
	return (i);
}

void	cd_root(t_data *data)
{
	char	*home;

	home = get_var_content(data, "$HOME");
	chdir(home);
	data->cd.new_pwd_tmp = ft_strdup("PWD=/root");
	free(home);
}
