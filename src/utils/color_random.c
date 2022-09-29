/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_random.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 12:42:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/29 13:00:19 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*rnd_cl_code(int color[2])
{
	int		i;
	char	*str;

	i = 0;
	str = ft_calloc(3, sizeof(char));
	while (i <= 1)
	{
		if (color[i] == 0)
			str[i] = 'w';
		else if (color[i] == 1)
			str[i] = 'r';
		else if (color[i] == 2)
			str[i] = 'g';
		else if (color[i] == 3)
			str[i] = 'y';
		else if (color[i] == 4)
			str[i] = 'b';
		else if (color[i] == 5)
			str[i] = 'm';
		else if (color[i] == 6)
			str[i] = 'c';
		i++;
	}
	return (str);
}

static int	rnd_get_digits(int rn_pipe[2])
{
	char	*rn_buf;
	char	*rn_chars;
	int		ret;

	rn_buf = ft_calloc(20, sizeof(char));
	read(rn_pipe[0], rn_buf, 19);
	close(rn_pipe[1]);
	close(rn_pipe[0]);
	rn_chars = ft_substr(rn_buf, 17, 2);
	ret = ft_atoi(rn_chars);
	free (rn_buf);
	free (rn_chars);
	return (ret);
}

static int	rnd_fork(t_data *data)
{
	pid_t	pid;
	int		rn_pipe[2];
	char	*date_path;
	char	*rn_argv[3];
	char	*rnd_chars;

	pipe(rn_pipe);
	date_path = get_path(data, "date");
	rn_argv[0] = date_path;
	rn_argv[1] = "+%s%N";
	rn_argv[2] = NULL;
	pid = fork();
	if (pid == 0)
	{
		dup2(rn_pipe[1], 1);
		close(rn_pipe[1]);
		close(rn_pipe[0]);
		execve(date_path, rn_argv, data->envp);
	}
	waitpid(pid, NULL, 0);
	free(date_path);
	return (rnd_get_digits(rn_pipe));
}

static int	*rnd_cl_array(int rnd_int)
{
	int		i;
	int		j;
	int		k;
	int		*color;

	i = 0;
	color = ft_calloc(3, sizeof(int));
	color[0] = rnd_int / 10;
	color[1] = rnd_int % 10;
	while (i < 2)
	{
		j = 0;
		k = 1;
		while (j < color[i])
		{
			k++;
			if (k > 6)
				k = 1;
			j++;
		}
		color[i++] = k;
	}
	return (color);
}

bool	random_cl(t_data *data)
{
	int		rnd_int;
	int		*color;
	char	*str;

	rnd_int = rnd_fork(data);
	color = rnd_cl_array(rnd_int);
	str = rnd_cl_code(color);
	if (!ft_strchr(str, data->color.code[0][0])
		&& !ft_strchr(str, data->color.code[1][0])
		&& str[0] != str[1])
	{
		free (data->color.code[0]);
		free (data->color.code[1]);
		builtin_color(data, str);
		free(str);
		return (true);
	}
	free(str);
	return (false);
}

bool	builtin_rcl(t_data *data)
{
	data->flags->rndcl = !data->flags->rndcl;
	if (data->flags->rndcl)
		printf("random prompt colors enabled\n");
	else
		printf("random prompt colors disabled\n");
	return (true);
}