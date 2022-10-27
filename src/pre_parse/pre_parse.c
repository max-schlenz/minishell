/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:32:27 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/27 22:51:30 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pre_parse_insert_space(t_data *data, char *cmd, int index)
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

char	*pre_parse_rm_delim(t_data *data, char *cmd, int start, int end)
{
	char	*ret;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len - (start - end) + 1);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (i == start)
			i = end - 1;
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

static void	pre_parse_skip_delim(char *cmd, int *i, char delim)
{
	while (cmd[*i] == delim)
		(*i)++;
	if (!cmd[*i] && delim == ' ')
		(*i)++;
}

char	*pre_parse_skip_d(t_data *data, char *cmd, char delim)
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
			pre_parse_skip_delim(cmd, &data->pparse.j, delim);
			cmd = pre_parse_rm_delim(data, cmd, data->pparse.i, data->pparse.j);
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

	i = 0;
	ops = "|&><";
	cmd = pre_parse_skip_d(data, cmd, ' ');
	data->cmd = NULL;
	while (*ops)
	{
		i = 0;
		while (cmd[i])
		{
			if (cmd[i] == '\\')
			{
				i++;
				continue ;
			}
			pre_parse_check_ops(data, &cmd, *ops, i);
			i++;
		}
		ops++;
	}
	if (i > 2 && !ft_strncmp(cmd + i - 2, ";;", 2))
		return (err_msg(syntax_err_type(data, ';', 2, 0)), free(cmd), NULL);
	return (cmd);
}
