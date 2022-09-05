/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:12:51 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/05 16:31:18 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*check_esc_var_quo(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
			return ((char *)s + i);
		i++;
	}
	if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
		return ((char *)s + i);
	return (0);
}

size_t	strlen_path(const char *c)
{
	int	i;

	i = 0;
	while (c[i] && (ft_isalnum(c[i]) || c[i] == '$'))
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

void	realloc_envp(t_data *data, int flag)
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
	new[i] = ft_strdup(data->args[1]);
	new[i + 1] = NULL;
	data->envp = new;
}
