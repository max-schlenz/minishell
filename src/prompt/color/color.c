/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 10:27:33 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 11:51:40 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	color_choice(t_data *data, int i, int j)
{
	if (data->color.code[i][0] == 'w')
		data->color.prompt[j] = ft_strdup(WHITE);
	else if (data->color.code[i][0] == 'r')
		data->color.prompt[j] = ft_strdup(RED);
	else if (data->color.code[i][0] == 'g')
		data->color.prompt[j] = ft_strdup(GREEN);
	else if (data->color.code[i][0] == 'y')
		data->color.prompt[j] = ft_strdup(YELLOW);
	else if (data->color.code[i][0] == 'b')
		data->color.prompt[j] = ft_strdup(BLUE);
	else if (data->color.code[i][0] == 'm')
		data->color.prompt[j] = ft_strdup(MAGENTA);
	else if (data->color.code[i][0] == 'c')
		data->color.prompt[j] = ft_strdup(CYAN);
	else
		return (false);
	return (true);
}

static void	build_prompt(t_data *data)
{
	int	fd;

	fd = open(data->cfg, O_RDWR, 0644);
	if (!fd || access(data->cfg, F_OK))
		cleanup(data, E_RW);
	data->color.cfg_str_pf = ft_strdup("COLOR=");
	data->color.cfg_str_cl
		= ft_strjoin(data->color.cfg_str_pf, data->color.code[0]);
	data->color.cfg_str
		= ft_strjoin(data->color.cfg_str_cl, data->color.code[1]);
	write(fd, data->color.cfg_str, ft_strlen(data->color.cfg_str));
	data->color.prompt_tmp
		= ft_strjoin(data->color.prompt[0], data->color.prompt[1]);
	data->color.prompt_tmp2
		= ft_strjoin(data->color.prompt[2], data->color.prompt[3]);
	free (data->prompt);
	data->prompt = ft_strjoin(data->color.prompt_tmp, data->color.prompt_tmp2);
}

static void	color_prompt(t_data *data, char *cfg)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < 2)
	{
		if (!cfg)
		{
			if (!i)
				data->color.code[i] = readline("enter color code 1: ");
			else
				data->color.code[i] = readline("enter color code 2: ");
			data->color.code[i][1] = '\0';
		}
		else
			data->color.code[i] = ft_substr(cfg, i, 1);
		if (data->color.code[i])
		{
			if (!color_choice(data, i, j))
				continue ;
		}
		j = 2;
		i++;
	}
}

//	builtin prompt color settings
//	color
bool	builtin_color(t_data *data, char *cfg)
{
	data->color.prompt[1] = ft_strdup("mini");
	data->color.prompt[3]
		= ft_strdup("shell\x01\033[0m\x02\x01\033[38;5;239m\x02 > [");
	if (!cfg)
		color_help();
	color_prompt(data, cfg);
	build_prompt(data);
	color_cleanup(data);
	data->exit_status = 0;
	return (true);
}
