/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:12:51 by tdehne            #+#    #+#             */
/*   Updated: 2022/10/05 08:40:49 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char *strrepc(char *cmd, char to_rep, char rep_with)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == to_rep)
			cmd[i] = rep_with;
		i++;
	}
	return (cmd);
}

size_t	strlen_expv(const char *c)
{
	int	i;

	i = 0;
	if (c[0] && c[0] == '$')
		i++;
	while (c[i] && (isnumeric(c[i]) || c[i] == '~'))
		i++;
	return (i);
}

size_t	strlen_path(const char *c)
{
	int	i;

	i = 0;
	if (c[0] && c[0] == '$')
		i++;
	while (c[i] && (ft_isalnum(c[i]) || c[i] == '~'))
		i++;
	return (i);
}

size_t	strlen_var(const char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '=')
		i++;
	return (i);
}

void	realloc_envp(t_data *data, char *newv, int flag)
{
	char	**new;
	int		i;

	if (flag)
		data->counter_env++;
	else
		data->counter_env--;
	new = ft_calloc(data->counter_env + 1, sizeof(char *));
	i = 0;
	while (data->envp[i])
	{
		new[i] = ft_strdup(data->envp[i]);
		free(data->envp[i]);
		i++;
	}
	free(data->envp);
	new[i] = ft_strdup(newv);
	new[i + 1] = NULL;
	data->envp = new;
}

bool	builtin_history(t_data *data)
{
	char	tmp_cl[9];
	char	tmp_print;
	int		fd;
	bool	flag_hist;
	bool	flag_print;
	
	if (data->argv[1] && !ft_strncmp(data->argv[1], "-c", 2))
	{
		usleep(1000);
		fd = open(CFG, O_RDONLY);
		read(fd, tmp_cl, 8);
		tmp_cl[8] = '\0';
		close (fd);
		fd = open(CFG, O_RDWR | O_TRUNC);
		write(fd, tmp_cl, 8);
		write(fd, "\n", 1);
		close (fd);
	}
	else
	{
		flag_hist = false;
		flag_print = false;
		fd = open(CFG, O_RDONLY);
		while (read(fd, &tmp_print, 1))
		{
			if (tmp_print == '\n' && !flag_hist)
				flag_hist = true;
			if (tmp_print == '=' && flag_hist)
			{
				read(fd, &tmp_print, 1);
				flag_print = true;
			}
			if (flag_print)
				write(1, &tmp_print, 1);
			if (tmp_print == '\n' && flag_hist)
				flag_print = false;
		}
		close (fd);
	}
	return (true);
}