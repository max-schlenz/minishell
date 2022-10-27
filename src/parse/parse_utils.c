/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:12:51 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/15 12:14:40 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

size_t	strlen_path(const char *c)
{
	int	i;

	i = 0;
	if (c[0] && c[0] == '$')
		i++;
	while (c[i] && (ft_isalnum(c[i]) || c[i] == '~'))
		i++;
	return (i);
}

size_t	strlen_var(const char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '=')
		i++;
	return (i);
}

void	realloc_envp(t_data *data, char *newv, int flag)
{
	char	**new;
	int		i;

	if (flag)
		data->counter_env++;
	else
		data->counter_env--;
	new = ft_calloc(data->counter_env + 1, sizeof(char *));
	i = 0;
	while (data->envp[i])
	{
		new[i] = ft_strdup(data->envp[i]);
		free(data->envp[i]);
		i++;
	}
	free(data->envp);
	new[i] = ft_strdup(newv);
	new[i + 1] = NULL;
	data->envp = new;
}

char	*strjoin_nl(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (0);
	str = malloc((ft_strlen(s1)) + (ft_strlen(s2)) + 2);
	if (!str)
		return (0);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\n';
	str[i + 1] = '\0';
	return (str);
}
