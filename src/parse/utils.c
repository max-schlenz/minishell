/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:12:51 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/17 14:22:53 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char *strrepc(char *cmd, char to_rep, char rep_with)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == to_rep)
			cmd[i] = rep_with;
		i++;
	}
	return (cmd);
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
	free(data->envp);
	new[i] = ft_strdup(data->argv[1]);
	new[i + 1] = NULL;
	data->envp = new;
}
