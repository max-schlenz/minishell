/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:32:27 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/12 16:13:01 by tdehne           ###   ########.fr       */
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
		ret[j] = cmd[i];
		if (i == index)
			ret[++j] = ' ';
		i++;
		j++;
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
		ret[j] = cmd[i];
		i++;
		j++;
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
			cmd = delete_spaces(cmd, i, j);
		}
		i++;
	}
	return (cmd);
}

char *pre_parse(char *cmd)
{
	int	i;
	char *ops;

 	ops = ft_strdup("|&><");
	i = 0;
	cmd = skip_s(cmd);
	while (*ops)
	{
		i = 0;
		while (cmd[i])
		{
			if (cmd[i + 1] && cmd[i] != ' ' && cmd[i] != *ops && cmd[i + 1] == *ops)
				cmd = insert_space(cmd, i);
			else if (cmd[i + 1] && cmd[i + 1] != ' ' && cmd[i + 1] != *ops && cmd[i] == *ops)
				cmd = insert_space(cmd, i);
			i++;
		}
		ops++;
	}
	return (cmd);
}