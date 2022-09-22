/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/20 10:50:16 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_syntax_first_char(t_data *data, char *cmd)
{
	int		i;
	char	*chars;
	char	*chars_stdin;
	char	*err;
	char	err_c[3];

	i = 0;
	chars = ft_strdup("|&>~");
	while (chars[i])
	{
		if (cmd[0] == chars[i])
		{
			if (chars[i] == '~')
				data->exit_status = 126;
			else
				data->exit_status = 2;
			err_c[0] = cmd[0];
			err_c[1] = '\0';
			err = strjoin_nl("Syntax error: ", err_c);
			write (2, err, ft_strlen(err));
			free (err);
			free (chars);
			return (false);
		}
		i++;
	}
	i = 0;
	if ((cmd[i] == '<')
	|| (cmd[i] == '.'))
	{
		i++;
		while (cmd[i] == ' ')
			i++;
		if (!cmd[i])
		{
			data->exit_status = 2;
			err_c[0] = cmd[i - 1];
			err_c[1] = '\0';
			err = strjoin_nl("Syntax error: ", err_c);
			write (2, err, ft_strlen(err));
			free (err);
			free (chars);
			return (false);
		}
	}
	if (ft_strlen(cmd) <= 2 && !ft_strncmp(cmd, "<<", 2))
	{
		data->exit_status = 2;
		err_c[0] = '<';
		err_c[1] = '<';
		err_c[2] = '\0';
		err = strjoin_nl("Syntax error: ", err_c);
		write (2, err, ft_strlen(err));
		free (err);
		free (chars);
		return (false);
	}
	if (ft_strlen(cmd) <= 2 && !ft_strncmp(cmd, "..", 2))
	{
		data->exit_status = 127;
		err_c[0] = '.';
		err_c[1] = '.';
		err_c[2] = '\0';
		err = strjoin_nl("Syntax error: ", err_c);
		write (2, err, ft_strlen(err));
		free (err);
		free (chars);
		return (false);
	}
	free (chars);
	return (true);
}

bool	check_syntax(t_data *data, char *cmd)
{
	int		i;
	int		j;
	char	*ops_supported;
	// char	*ops_unsupported;
	char	*err;
	char	err_c[2];

	ops_supported = ft_strdup("|&><");
	err = NULL;
	j = 0;
	while (ops_supported[j])
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
			if	(cmd[i]
			&&	cmd[i + 1]
			&&	cmd[i + 2]
			&&	cmd[i] == ops_supported[j]
			&&	cmd[i + 1] == ops_supported[j]
			&&	cmd[i + 2] == ops_supported[j])
			{
				err_c[0] = ops_supported[j];
				err_c[1] = '\0';
				err = strjoin_nl("Syntax error: ", err_c);
				write (2, err, ft_strlen(err));
				free (err);
				// printf("Syntax error: '%c' [%d]\n", ops_supported[j], i + 3);
				data->exit_status = 2;
				// write(2, "Syntax Error\n", 14);
				free (ops_supported);
				return (false);
			}
			i++;
		}
		j++;
	}
	free (ops_supported);
	return (true);
}

bool	syntax_err(t_data *data, char *cmd)
{
	int i = 0;
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