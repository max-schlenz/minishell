/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 13:15:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/11 13:35:46 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	check if prio is necessary
void	priorities(t_data *data, char **tmp_cmd, int *i)
{
	int	j;

	j = 0;
	if (data->argv[0] && (data->argv[0][0] == '(' || data->flags->bracket))
		prio(data, *tmp_cmd, i);
	while (data->argv[j])
	{
		data->argv[j] 
			= realloc_ptr(data->argv[j], ft_strtrim(data->argv[j], "()"), true);
		j++;
	}
}

//	check syntax of the command
bool	prompt_syntax_check(t_data *data, char **tmp_cmd)
{
	if (!check_syntax(data, *tmp_cmd) \
	|| !syntax_err(data, *tmp_cmd) \
	|| !check_syntax_first_char(data, *tmp_cmd) \
	|| !syntax_check_pipes_redirs(data, *tmp_cmd))
	{
		free_null(1, tmp_cmd);
		return (false);
	}
	return (true);
}
