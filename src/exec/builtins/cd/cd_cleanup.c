/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 09:37:54 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/05 10:04:32 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_cd(t_data *data)
{
	if (data->cd.path)
		free (data->cd.path);
	if (data->cd.path_tmp_bs)
		free (data->cd.path_tmp_bs);
	if (data->cd.path_tmp)
		free (data->cd.path_tmp);
	if (data->cd.path_tmp2)
		free (data->cd.path_tmp2);
	if (data->cd.new_pwd_tmp)
		free (data->cd.new_pwd_tmp);
	init_cd(data);
}

void	init_cd(t_data *data)
{
	data->cd.path = NULL;
	data->cd.path_tmp_bs = NULL;
	data->cd.path_tmp = NULL;
	data->cd.path_tmp2 = NULL;
	data->cd.new_pwd_tmp = NULL;
}

bool	cd_err(t_data *data)
{
	perror("Error");
	free_cd(data);
	data->exit_status = 1;
	if (data->flags->pipe)
		exit(0);
	return (true);
}

bool	cd_success(t_data *data, int i)
{
	free (data->envp[i]);
	data->envp[i] = ft_strdup(data->cd.new_pwd_tmp);
	free (data->cd.new_pwd_tmp);
	data->exit_status = 0;
	if (data->flags->pipe)
		exit(0);
	return (true);
}