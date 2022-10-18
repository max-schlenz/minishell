/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 15:15:46 by tdehne           ###   ########.fr       */
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
	chars = "|&>~";
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

static bool	syntax_err_consecutive(t_data *data, char *cmd, int *i, int *k)
{
	char	*ops;

	ops = "|&><";
	if (!ft_strncmp(cmd + (*i), "<<", 3))
	{
		(*i)++;
		return (true);
	}
	// while (ops[(*k)])
	// {
	// 	(*k)++;
	// }
	printf("in here\n");
	if (cmd[(*i) + 2] == ops[(*k)])
		return (err_msg(err_type(data, ops[(*k)], 2, 0)));
	return (true);
}

bool	syntax_err(t_data *data, char *cmd)
{
	int		i;
	int		j;
	int		k;
	char	*ops;

	i = 0;
	j = 0;
	k = 0;
	ops = "|&><";
	while (cmd[i] && cmd[i + 1] && cmd[i + 2])
	{
		while (ops[j])
		{
			if (cmd[i] == ops[j])
			{
				if (!syntax_err_consecutive(data, cmd, &i, &j))
					return (false);
			}
			k = 0;
			j++;
		}
		j = 0;
		i++;
	}
	return (true);
}
