/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:32:27 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/14 14:03:05 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*insert_space(t_data *data, char *cmd, int index)
{
	size_t	len;
	char	*ret;
	int		i;
	int		j;

	len = ft_strlen(cmd) + 1;
	ret = (char *)ft_calloc(sizeof(char), len + 1);
	i = 0;
	j = 0;
	while ((cmd)[i])
	{
		ret[j++] = (cmd)[i];
		if (i++ == index)
			ret[j++] = ' ';
	}
	free(cmd);
	data->cmd = NULL;
	return (ret);
}

char	*delete_delim(t_data *data, char *cmd, int start_space, int end_space)
{
	char	*ret;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len - (start_space - end_space) + 1);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (i == start_space)
			i = end_space - 1;
		ret[j++] = cmd[i];
		if (cmd[i] && cmd[i + 1])
			i++;
		else
			break ;
	}
	ret[j] = '\0';
	free(cmd);
	data->cmd = NULL;
	return (ret);
}

static void	skip_delim(char *cmd, int *i, char delim)
{
	while (cmd[*i] == delim)
		(*i)++;
	if (!cmd[*i] && delim == ' ')
		(*i)++;
}

char	*skip_d(t_data *data, char *cmd, char delim)
{
	char	*ret;

	data->pparse.i = 0;
	data->pparse.j = 0;
	data->pparse.f_dquote = false;
	data->pparse.f_squote = false;
	while (cmd[data->pparse.i])
	{
		if (cmd[data->pparse.i] == '\"' && !data->pparse.f_squote)
				data->pparse.f_dquote = !data->pparse.f_dquote;
		if (cmd[data->pparse.i] == '\'' && !data->pparse.f_dquote)
				data->pparse.f_squote = !data->pparse.f_squote;
		if (((cmd[data->pparse.i + 1] && cmd[data->pparse.i] == delim
					&& cmd[data->pparse.i + 1] == delim)
				|| (cmd[data->pparse.i] == delim && !cmd[data->pparse.i + 1]))
			&& !data->pparse.f_dquote && !data->pparse.f_squote)
		{
			data->pparse.j = data->pparse.i;
			skip_delim(cmd, &data->pparse.j, delim);
			cmd = delete_delim(data, cmd, data->pparse.i, data->pparse.j);
		}
		data->pparse.i++;
	}
	ret = strmv(cmd);
	return (ret);
}

char	*pre_parse(t_data *data, char *cmd)
{
	int		i;
	char	*ops;

	ops = "|&><";
	i = 0;
	cmd = skip_d(data, cmd, ' ');
	data->cmd = NULL;
	while (*ops)
	{
		i = 0;
		while (cmd[i])
		{
			if (cmd[i + 1] && cmd[i] != ' '
				&& cmd[i] != *ops && cmd[i + 1] == *ops)
				cmd = insert_space(data, cmd, i);
			else if (cmd[i + 1] && cmd[i + 1] != ' '
				&& cmd[i + 1] != *ops && cmd[i] == *ops)
				cmd = insert_space(data, cmd, i);
			i++;
		}
		ops++;
	}
	return (cmd);
}
