/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 22:36:57 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/25 10:58:56 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	rm_tmp_files(t_data *data)
{
	int		i;
	char	*tmp_name;
	char	*tmp_index;

	i = 0;
	while (i < data->heredoc_index)
	{
		tmp_index = ft_itoa(i);
		tmp_name = ft_strjoin(".heredoc_tmp", tmp_index);
		unlink(tmp_name);
		free_str (2, tmp_name, tmp_index);
		i++;
	}
}
