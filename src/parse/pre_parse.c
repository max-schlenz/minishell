/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 15:32:27 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/04 15:51:57 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *insert_space(t_data *data, char *cmd, int index)
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
	while(cmd[i])
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

// static void	skip_delim_iter(t_data *data, char *cmd, int *i, char delim)
// {
// 	bool	f_dquote;
// 	bool	f_squote;
// 	int		j;

// 	f_dquote = false;
// 	f_squote = false;
// 	j = 0;
// 	if (cmd[(*i)] == '\"' && !f_squote)
// 			f_dquote = !f_dquote;
// 	if (cmd[(*i)] == '\'' && !f_dquote)
// 			f_squote = !f_squote;
// 	if (!cmd[(*i) + 1] && cmd[(*i)] == delim && !f_dquote && !f_squote)
// 	{
// 		j = (*i);
// 		skip_delim(cmd, &j, delim);
// 		cmd = delete_delim(data, cmd, (*i), j);
// 	}
// 	(*i)++;
// }

// char	*skip_d(t_data *data, char *cmd, char delim)
// {
// 	int		i;
// 	char	*ret;

// 	i = 0;
// 	while (cmd[i])
// 		skip_delim_iter(data, cmd, &i, delim);
// 	ret = ft_strdup(cmd);
// 	free(cmd);
// 	data->cmd = NULL;
// 	return (ret);
// }

char	*skip_d(t_data *data, char *cmd, char delim)
{
	int		i;
	int		j;
	bool	f_dquote;
	bool	f_squote;
	char	*ret;

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
		if (((cmd[i + 1] && cmd[i] == delim && cmd[i + 1] == delim) || (cmd[i] == delim && !cmd[i + 1])) && !f_dquote && !f_squote)
		{
			j = i;
			skip_delim(cmd, &j, delim);
			cmd = delete_delim(data, cmd, i, j);
		}
		i++;
	}
	ret = ft_strdup(cmd);
	free(cmd);
	data->cmd = NULL;
	return (ret);
}

char	*pre_parse(t_data *data, char *cmd)
{
	int		i;
	char	*ops;

	ops = "|&><";
	i = 0;
	cmd = skip_d(data, cmd, ' ');
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
