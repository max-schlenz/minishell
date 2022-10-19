/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/19 11:18:32 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
}

int	strcmp_alnum(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] != '\0' && s2[i] != '\0'
		&& ft_isalnum(s1[i]) && ft_isalnum(s2[i]))
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
	i++;
	}
	return (0);
}

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

void	set_shlvl_env(t_data *data)
{
	int		shlvl;
	char	*shlvl_new;
	char	*envv;
	char	*pwd;

	envv = get_var_content(data, "$SHLVL");
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

void	parse_envp(t_data *data, char **envp)
{
	int	i;

	while (envp[data->counter_env])
		data->counter_env++;
	if (data->flags->noenv)
		data->counter_env = 3;
	data->envp = ft_calloc(data->counter_env + 1, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		data->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	data->envp[i] = NULL;
	parse_path(data);
	set_shlvl_env(data);
	sort_array(data);
}
