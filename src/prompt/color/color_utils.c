/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 22:38:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/31 11:48:14 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	color_cleanup(t_data *data)
{
	free_null (9, &data->color.prompt[0],
		&data->color.prompt[1],
		&data->color.prompt[2],
		&data->color.prompt[3],
		&data->color.prompt_tmp,
		&data->color.prompt_tmp2,
		&data->color.cfg_str_pf,
		&data->color.cfg_str_cl,
		&data->color.cfg_str);
}

void	color_help(void)
{
	printf("change the color of your prompt!\n\n \
		w - \033[0mwhite\033[0m\n \
		r - \033[31;1mred\033[0m\n \
		g - \033[32;1mgreen\033[0m\n \
		y - \033[33;1myellow\033[0m\n \
		b - \033[34;1mblue\033[0m\n \
		m - \033[35;1mmagenta\033[0m\n \
		c - \033[36;1mcyan\033[0m\n\n");
}
