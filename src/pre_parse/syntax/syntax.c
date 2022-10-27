/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/26 16:34:16 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	synax_err_special_chars(t_data *data, char *cmd)
{
	int	i;

	i = 0;
	if ((cmd[i] == '<') || (cmd[i] == '.'))
	{
		i++;
		while (cmd[i] == ' ')
			i++;
		if (!cmd[i])
			return (err_msg(err_type(data, cmd[i - 1], 2, 0)));
	}
	return (true);
}

bool	check_syntax_first_char(t_data *data, char *cmd)
{
	int		i;
	char	*chars;

	i = 0;
	chars = "|&~";
	while (chars[i])
	{
		if (cmd[0] == chars[i])
		{
			if (chars[i] == '~')
				return (err_msg(err_type(data, cmd[0], 126, 0)));
			else
				return (err_msg(err_type(data, cmd[0], 2, 0)));
		}
		i++;
	}
	if (!synax_err_special_chars(data, cmd))
		return (false);
	if (ft_strlen(cmd) <= 2 && !ft_strncmp(cmd, "<<", 2))
		return (err_msg(err_type(data, '<', 2, 1)));
	if (ft_strlen(cmd) <= 2 && !ft_strncmp(cmd, "..", 2))
		return (err_msg(err_type(data, '.', 127, 1)));
	return (true);
}

bool	check_syntax(t_data *data, char *cmd)
{
	int				i;
	char			*ops;

	ops = "|&><";
	while (*ops)
	{
		i = 0;
		while (cmd[i] && cmd[i + 1] && cmd[i + 2])
		{
			if (cmd[i] == '\'' || cmd[i] == '\"')
			{
				i++;
				while ((cmd[i] && cmd[i + 1] && cmd[i + 2]))
				{
					if (check_syntax_iter(cmd, &i))
						continue ;
				}
			}
			if (check_syntax_helper(cmd, ops, i))
				return (err_msg(err_type(data, *ops, 2, 0)));
			i++;
		}
		ops++;
	}
	return (true);
}

bool	syntax_err_consecutive(t_data *data, char *cmd, int *i, int *j)
{
	char	*ops;
	int		k;

	ops = "|&><";
	k = 0;
	if (!ft_strncmp(cmd + (*i), "<<", 2))
	{
		(*i)++;
		return (true);
	}
	if (!ft_strncmp(cmd + (*i), ";;", 2))
		return (err_msg(err_type(data, ';', 2, 0)));
	while (ops[k])
	{
		if ((cmd[(*i)] == '&' || cmd[(*i)] == '|')
			&& cmd[(*i) + 2] == ops[(*j)])
			return (err_msg(err_type(data, ops[(*j)], 2, 0)));
		else if ((cmd[(*i)] == '&' || cmd[(*i)] == '|'))
			return (true);
		else if (cmd[(*i) + 2] == ops[k])
			return (err_msg(err_type(data, ops[k], 2, 0)));
		k++;
	}
	return (true);
}

bool	syntax_check_pipes_redirs(t_data *data, char *cmd)
{
	int		i;
	int		j;
	int		k;
	char	*ops;

	j = 0;
	ops = "|><";
	while (ops[j])
	{
		i = 0;
		while (cmd[i])
		{
			if (cmd[i] == ops[j])
			{
				k = i + 1;
				while (cmd[k] && cmd[k] == ' ')
					k++;
				if (!cmd[k])
					return (err_msg(err_type(data, *ops, 2, 0)));
			}
			i++;
		}
		j++;
	}
	return (true);
}
