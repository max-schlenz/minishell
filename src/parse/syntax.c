/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 10:10:51 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/17 14:32:08 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_syntax(char *cmd)
{
	int		i;
	int		j;
	char	*ops_supported;
	char	*ops_unsupported;

	ops_supported = ft_strdup("|&><");
	ops_unsupported = ft_strdup("{");

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
				printf("Syntax error: '%c' [%d]\n", ops_supported[j], i + 3);
				free (ops_supported);
				free (ops_unsupported);
				return (false);
			}
			i++;
		}
		j++;
	}
	free (ops_supported);
	j = 0;
	while (ops_unsupported[j])
	{
		i = 0;
		while (cmd[i])
		{
			if (cmd[i] == '\'' || cmd[i] == '\"')
			{
				i++;
				while (cmd[i])
				{
					if (cmd[i] == '\'' || cmd[i] == '\"')
						break ;
					i++;
				}
			}
			if	(cmd[i] == ops_unsupported[j])
			{
				printf("Syntax error: '%c' [%d] unsupported operation\n", ops_unsupported[j], i + 3);
				free (ops_unsupported);
				return (false);
			}
			i++;
		}
		j++;
	}
	free (ops_unsupported);
	return (true);
}

bool	syntax_err(char *cmd)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char *ops_supported = ft_strdup("|&><");
	
	while (cmd[i] && cmd[i + 1] && cmd[i + 2])
	{
		while (ops_supported[j])
		{
			if (cmd[i] == ops_supported[j])
			{
				while (ops_supported[k])
				{
					if (cmd[i + 2] == ops_supported[k])
					{
						printf("Syntax error: '%c' [%d]\n", ops_supported[k], i);
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