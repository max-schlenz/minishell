/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 17:01:42 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/17 14:25:18 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_path(t_data *data)
{
	int		i;

	i = 0;
	if (data->path)
	{
		free_array(data->path);
		free (data->path);
	}
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5))
		i++;
	data->path = ft_split(data->envp[i], ':');
}

int		strcmp_alnum(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] != '\0' && s2[i] != '\0' && ft_isalnum(s1[i]) && ft_isalnum(s2[i]))
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

	i = 0;
	j = 0;
	while (j < data->counter_env * data->counter_env)
	{
		while (i < data->counter_env)
		{
			len = ft_strlen(data->envp[i]);
			if (len > ft_strlen(data->envp[i + 1]))
				len = ft_strlen(data->envp[i + 1]);
			if (ft_strncmp(data->envp[i], data->envp[i + 1], len) > 0)
			{
				tmp = data->envp[i];
				data->envp[i] = data->envp[i + 1];
				data->envp[i + 1]= tmp;
				break;
			}
			i++;
		}
		i = 0;
		j++;
	}
}

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
	sort_array(data);
	parse_path(data);
}
