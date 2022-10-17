/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 08:39:23 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/17 20:39:41 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	cd_minus(t_data *data, int i)
{
	int	j;

	j = ft_strlen(data->envp[i] + 4);
	while (j && data->envp[i][j] != '/')
		j--;
	data->cd.path_tmp = ft_strdup("");
	data->cd.path_tmp_bs = ft_strdup("");
	data->cd.path = ft_substr(data->envp[i], 4, j - 4);
	data->cd.new_pwd_tmp = ft_strjoin("PWD=", data->cd.path);
}

static void	cd_build_path(t_data *data, int i, bool absolute)
{
	if (absolute)
	{
		data->cd.path = ft_strdup(data->cd.path_tmp2);
		data->cd.new_pwd_tmp = ft_strjoin("PWD=", data->cd.path_tmp2);
	}
	else
	{
		data->cd.path_tmp = ft_strdup(data->envp[i] + 4);
		data->cd.path_tmp_bs = ft_strjoin(data->cd.path_tmp, "/");
		data->cd.path = ft_strjoin(data->cd.path_tmp_bs, data->cd.path_tmp2);
		data->cd.new_pwd_tmp = ft_strjoin("PWD=", data->cd.path);
	}
}

static void	cd_chdir(t_data *data, int i)
{
	int	j;

	j = 0;
	chdir(data->cd.path);
	free (data->cd.path);
	data->cd.path = NULL;
	if (ft_strncmp(data->argv[1], "..", 3) && (!data->cd.new_pwd_tmp))
	{
		if (data->envp[i][ft_strlen(data->envp[i]) - 1] != '/')
			data->cd.path = ft_strjoin("/", data->argv[1]);
		else
			data->cd.path = ft_strdup(data->argv[1]);
		data->cd.new_pwd_tmp = ft_strjoin(data->envp[i], data->cd.path);
	}
	else if (!data->cd.new_pwd_tmp)
	{
		j = ft_strlen(data->envp[i]) - 1;
		while (data->envp[i][j] && data->envp[i][j] != '/')
			j--;
		data->cd.new_pwd_tmp = ft_substr(data->envp[i], 0, j);
	}
}

static bool	cd_def(t_data *data, int i)
{
	data->cd.path_tmp2 = ft_strtrim(data->argv[1], " ");
	if (!ft_strncmp(data->argv[1], "-", 2))
		cd_minus(data, i);
	else if (data->argv[1][0] != '/')
		cd_build_path(data, i, false);
	else
		cd_build_path(data, i, true);
	if (!access(data->cd.path, F_OK))
		cd_chdir(data, i);
	else
	{
		free_cd(data);
		return (false);
	}
	free_cd(data);
	return (true);
}

bool	builtin_cd(t_data *data)
{
	int		index_pwd;

	init_cd(data, &index_pwd);
	if (!data->argv[1])
		cd_root(data);
	else if ((data->flags->pipe && !data->pid) || !data->flags->pipe)
	{
		if (data->flags->pipe)
			redirs_pipes(data);
		index_pwd = cd_find_pwd(data);
		if (data->argv[1] && !ft_strncmp(data->argv[1], ".", 2))
		{
			free_cd(data);
			return (true);
		}
		if (data->argv[1] && !cd_def(data, index_pwd))
			return (cd_err(data));
	}
	return (cd_success(data, index_pwd));
}
