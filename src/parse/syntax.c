/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/13 13:38:40 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	*err_msg(t_data *data, char *err_c)
{
	char	*err;

	err = strjoin_nl("Syntax error: ", err_c);
	write (2, err, ft_strlen(err));
	free (err);
	free (err_c);
	return (false);
}

static char	*err_type(t_data *data, char c, int exit_status, int flag)
{
	char	err_c[3];

	if (!flag)
	{
		err_c[0] = c;
		err_c[1] = '\0';
	}
	else
	{
		err_c[0] = c;
		err_c[1] = c;
		err_c[2] = '\0';
	}
	data->exit_status = exit_status;
	return (ft_strdup(err_c));
}

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
			return (err_msg(data, err_type(data, cmd[i - 1], 2, 0)));
	}
	return (true);
}

bool	check_syntax_first_char(t_data *data, char *cmd)
{
	int		i;
	char	*chars;
	char	err_c[3];

	i = 0;
	chars = "|&>~";
	while (chars[i])
	{
		if (cmd[0] == chars[i])
		{
			if (chars[i] == '~')
				return (err_msg(data, err_type(data, cmd[0], 126, 0)));
			else
				return (err_msg(data, err_type(data, cmd[0], 2, 0)));
		}
		i++;
	}
	if (!synax_err_special_chars(data, cmd))
		return (false);
	if (ft_strlen(cmd) <= 2 && !ft_strncmp(cmd, "<<", 2))
		return (err_msg(data, err_type(data, '<', 2, 1)));
	if (ft_strlen(cmd) <= 2 && !ft_strncmp(cmd, "..", 2))
		return (err_msg(data, err_type(data, '.', 127, 1)));
	return (true);
}

bool	check_syntax(t_data *data, char *cmd)
{
	int			i;
	static char	*ops_supported = "|&><";

	while (*ops_supported)
	{
		i = 0;
		while (cmd[i] && cmd[i + 1] && cmd[i + 2])
		{
			if (cmd[i] == '\'' || cmd[i] == '\"')
			{
				i++;
				while (cmd[i] && cmd[i + 1] && cmd[i + 2])
				{
					if (cmd[i] == '\'' || cmd[i++] == '\"')
						break ;
					i++;
				}
			}
			if (cmd[i] && cmd[i + 1] && cmd[i + 2] && cmd[i] == *ops_supported
				&& cmd[i + 1] == *ops_supported && cmd[i + 2] == *ops_supported)
				return (err_msg(data, err_type(data, *ops_supported, 2, 0)));
			i++;
		}
		*ops_supported++;
	}
	return (true);
}

bool	syntax_err(t_data *data, char *cmd)
{
	int		i = 0;
	int j = 0;
	int k = 0;
	char	*err;
	char	err_c[2];
	char *ops_supported = ft_strdup("|&><");
	
	while (cmd[i] && cmd[i + 1] && cmd[i + 2])
	{
		while (ops_supported[j])
		{
			if (cmd[i] == ops_supported[j])
			{
				if (ft_strncmp(cmd + i, "<<", 3))
				{
					i++;
					continue ;
				}
				while (ops_supported[k])
				{
					if (cmd[i + 2] == ops_supported[k])
					{
						// printf("Syntax error: '%c' [%d]\n", ops_supported[k], i);
						data->exit_status = 2;
						err_c[0] = ops_supported[k];
						err_c[1] = '\0';
						err = strjoin_nl("Syntax error: ", err_c);
						write (2, err, ft_strlen(err));
						free (err);
						free (ops_supported);
						return (false);
					}
					k++;
				}
			}
			k = 0;
			j++;
		}
		j = 0;
		i++;
	}
	free (ops_supported);
	return (true);
}