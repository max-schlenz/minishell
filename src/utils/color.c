/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 10:27:33 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/27 10:54:19 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	color_cleanup(t_data *data)
{
	free (data->color.prompt[0]);
	free (data->color.prompt[1]);
	free (data->color.prompt[2]);
	free (data->color.prompt[3]);
	free (data->color.code[0]);
	free (data->color.code[1]);
	free (data->color.prompt_tmp);
	free (data->color.prompt_tmp2);
	free (data->color.cfg_str_pf);
	free (data->color.cfg_str_cl);
	free (data->color.cfg_str);
}

static void	color_help(void)
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

	fd = open(".mscfg", O_RDWR, 0644);
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
