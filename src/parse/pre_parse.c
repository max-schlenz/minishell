/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:32:27 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/15 10:47:23 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *insert_space(char *cmd, int index)
{
	size_t	len;
	char	*ret;
	int		i;
	int		j;

	len = ft_strlen(cmd) + 1;
	ret = (char *)ft_calloc(sizeof(char), len + 1);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		ret[j++] = cmd[i];
		if (i++ == index)
			ret[j++] = ' ';
	}
	return (ret);
}


char *delete_spaces(char *cmd, int start_space, int end_space)
{
	char	*ret;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(cmd);
	ret = (char *)ft_calloc(sizeof(char), len - (start_space - end_space) + 1);
	i = 0;
	j = 0;
	while(cmd[i])
	{
		if (i == start_space)
			i = end_space - 1;
		ret[j++] = cmd[i++];
	}
	ret[j] = '\0';
	return (ret);
}

char *skip_s(char *cmd)
{
	int		i;
	int		j;
	bool	f_dquote;
	bool	f_squote;
	char	*tmp;

	i = 0;
	j = 0;
	f_dquote = false;
	f_squote = false;
	while (cmd[i])
	{
		if (cmd[i] == '\"' && !f_squote)
				f_dquote = !f_dquote;
		if (cmd[i] == '\'' && !f_dquote)
				f_squote = !f_squote;
		if (cmd[i + 1] && cmd[i] == ' ' && cmd[i + 1] == ' ' && !f_dquote && !f_squote)
		{
			j = i;
			skip_spaces(cmd, &j);
			tmp = delete_spaces(cmd, i, j);
			free (cmd);
			cmd = tmp;
		}
		i++;
	}
	return (cmd);
}

char *pre_parse(t_data *data, char *cmd)
{
	int	i;
	int	j;
	char *ops;
	char *tmp;
	char *tmp2;

 	ops = ft_strdup("|&><");
	i = 0;
	j = 0;
	tmp = skip_s(cmd);
	tmp2 = NULL;
	while (ops[j])
	{
		i = 0;
		while (tmp[i])
		{
			if (tmp[i + 1] && tmp[i] != ' ' && tmp[i] != ops[j] && tmp[i + 1] == ops[j])
			{
				tmp2 = insert_space(tmp, i);
				free (tmp);
			}
			else if (tmp[i + 1] && tmp[i + 1] != ' ' && tmp[i + 1] != ops[j] && tmp[i] == ops[j])
			{
				tmp2 = insert_space(tmp, i);
				free (tmp);
			}
			i++;
		}
		j++;
	}
	free (ops);
	if (tmp2)
		return (tmp2);
	return (tmp);
}
