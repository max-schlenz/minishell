/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 09:37:25 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/23 11:22:04 by mschlenz         ###   ########.fr       */
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

	home = expand_get_var_content(data, "$HOME");
	chdir(home);
	data->cd.new_pwd_tmp = ft_strdup("PWD=/root");
	free(home);
}

void	cd_set_oldpwd(t_data *data)
{
	char	*oldpwd;

	oldpwd = merge_str(2, ft_strdup("OLDPWD="), getcwd(NULL, 0));
	builtin_export(data, oldpwd);
	free_str(1, oldpwd);
}
