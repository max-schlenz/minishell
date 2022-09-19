/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/18 14:42:45 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_syntax_first_char(t_data *data)
{
	int		i;
	char	*chars;
	char	*chars_stdin;

	i = 0;
	chars = ft_strdup("|&>~");
	while (chars[i])
	{
		if (data->cmd[0] == chars[i])
		{
			if (chars[i] == '~')
				data->exit_status = 126;
			else
				data->exit_status = 2;
			write(2, "Syntax Error\n", 14);
			free (chars);
			return (false);
		}
		i++;
	}
	i = 0;
	if ((data->cmd[i] == '<')
	|| (data->cmd[i] == '.'))
	{
		i++;
		while (data->cmd[i] == ' ')
			i++;
		if (!data->cmd[i])
		{
			data->exit_status = 2;
			write(2, "Syntax Error\n", 14);
			free (chars);
			return (false);
		}
	}
	if (ft_strlen(data->cmd) <= 2 && !ft_strncmp(data->cmd, "<<", 2))
	{
		data->exit_status = 2;
		write(2, "Syntax Error\n", 14);
		free (chars);
		return (false);
	}
	if (ft_strlen(data->cmd) <= 2 && !ft_strncmp(data->cmd, "..", 2))
	{
		data->exit_status = 127;
		write(2, "Syntax Error\n", 14);
		free (chars);
		return (false);
	}
	free (chars);
	return (true);
}

bool	check_syntax(t_data *data)
{
	int		i;
	int		j;
	char	*ops_supported;
	// char	*ops_unsupported;
	char	*err;

	ops_supported = ft_strdup("|&><");
	err = NULL;
	j = 0;
	while (ops_supported[j])
	{
		i = 0;
		while (data->cmd[i] && data->cmd[i + 1] && data->cmd[i + 2])
		{
			if (data->cmd[i] == '\'' || data->cmd[i] == '\"')
			{
				i++;
				while (data->cmd[i] && data->cmd[i + 1] && data->cmd[i + 2])
				{
					if (data->cmd[i] == '\'' || data->cmd[i++] == '\"')
						break ;
					i++;
				}
			}
			if	(data->cmd[i]
			&&	data->cmd[i + 1]
			&&	data->cmd[i + 2]
			&&	data->cmd[i] == ops_supported[j]
			&&	data->cmd[i + 1] == ops_supported[j]
			&&	data->cmd[i + 2] == ops_supported[j])
			{
				// printf("Syntax error: '%c' [%d]\n", ops_supported[j], i + 3);
				data->exit_status = 2;
				write(2, "Syntax Error\n", 14);
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

bool	syntax_err(t_data *data)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char *ops_supported = ft_strdup("|&><");
	
	while (data->cmd[i] && data->cmd[i + 1] && data->cmd[i + 2])
	{
		while (ops_supported[j])
		{
			if (data->cmd[i] == ops_supported[j])
			{
				while (ops_supported[k])
				{
					if (data->cmd[i + 2] == ops_supported[k])
					{
						// printf("Syntax error: '%c' [%d]\n", ops_supported[k], i);
						data->exit_status = 2;
						write(2, "Syntax Error\n", 14);
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