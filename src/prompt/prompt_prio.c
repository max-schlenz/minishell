/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prio.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 13:49:28 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/19 15:50:22 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	last_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (str[i - 1]);
}

static void	prio_free_argv(t_data *data)
{
	free_array(data->argv);
	data->argv = ft_calloc(1, sizeof(char **));
	data->argv[0] = NULL;
}

//	evaluates if execution should take place
void	prio(t_data *data, char *cmd, int *i)
{
	int		j;
	char	*tmp;

	j = 0;
	data->flags->bracket = true;
	if ((data->flags->and && data->exit_status) \
	|| (data->flags->or && !data->exit_status))
	{
		data->flags->prio = true;
		prio_free_argv(data);
		while (cmd[*i] && cmd[*i] != ')' && cmd[*i] != '|' && cmd[*i] != '&')
			(*i)++;
	}
	else
	{
		while (data->argv[j])
		{
			if (last_char(data->argv[j]) == ')')
				data->flags->bracket = false;
			tmp = ft_strtrim(data->argv[j], "()");
			if (data->argv[j])
				free (data->argv[j]);
			data->argv[j++] = tmp;
		}
	}
}
