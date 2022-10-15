/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 13:15:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/15 13:39:37 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	prompt_priorisation(t_data *data, char **tmp_cmd, int *i)
{
	if (data->argv[0] && (data->argv[0][0] == '(' || data->flags->bracket))
		prio(data, *tmp_cmd, i);
}
