/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/02 16:47:49 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	parses envp path into minishell path array
void	parse_path(t_data *data)
{
	int		i;

	i = 0;
	if (data->path)
		free_array(data->path);
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	if (data->flags->noenv)
	{
		data->path = ft_calloc(2, sizeof(char **));
		data->path[0] = ft_strdup("");
		data->path[1] = NULL;
	}
	else
		data->path = ft_split(data->envp[i], ':');
	if (data->path && data->path[0] && !ft_strncmp(data->path[0], "PATH=", 5))
		data->path[0] = realloc_ptr(data->path[0],
				ft_strtrim(data->path[0], "PATH="), true);
}

//	sort array lexicographically
void	sort_array(t_data *data)
{
	char	*tmp;
	size_t	len;
	int		i;
	int		j;

	j = 0;
	while (j < data->counter_env * data->counter_env)
	{
		i = 0;
		while (i < data->counter_env)
		{
			len = ft_strlen(data->envp[i]);
			if (len > ft_strlen(data->envp[i + 1]))
				len = ft_strlen(data->envp[i + 1]);
			if (ft_strncmp(data->envp[i], data->envp[i + 1], len) > 0)
			{
				tmp = data->envp[i];
				data->envp[i] = data->envp[i + 1];
				data->envp[i + 1] = tmp;
				break ;
			}
			i++;
		}
		j++;
	}
}

//	sets some default env variables
void	init_envp(t_data *data)
{
	int		shlvl;
	char	*shlvl_new;
	char	*envv;
	char	*pwd;

	envv = expand_get_var_content(data, "$SHLVL");
	shlvl = ft_atoi(envv) + 1;
	free (envv);
	shlvl_new = ft_itoa(shlvl);
	envv = ft_strjoin("SHLVL=", shlvl_new);
	free (shlvl_new);
	builtin_export(data, envv);
	free (envv);
	if (data->flags->macos)
		builtin_export(data, "_=/usr/bin/env");
	else
		builtin_export(data, "_=/usr/sbin/env");
	pwd = getcwd(NULL, 0);
	envv = ft_strjoin("PWD=", pwd);
	free(pwd);
	builtin_export(data, envv);
	builtin_export(data, "OLDPWD");
	free(envv);
}

//	parse system envp into minishell envp array
void	parse_envp(t_data *data, char **envp)
{
	int	i;

	while (envp[data->counter_env])
		data->counter_env++;
	data->envp = ft_calloc(data->counter_env + 1, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
	parse_path(data);
	init_envp(data);
	sort_array(data);
}
