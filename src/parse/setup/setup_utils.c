/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:23:56 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 13:32:23 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	setup_alloc_argv(t_data *data, char *cmd)
{
	int		mem;
	int		i;

	mem = 1;
	i = 0;
	setup_reset_flags(data);
	while (cmd[i])
	{
		setup_esc(data, cmd, &i);
		setup_qflags(data, cmd, &i);
		setup_subshell(data, cmd, &i);
		if (!data->flags->f_dquote && !data->flags->f_squote
			&& cmd[i] == ' ' && cmd[i + 1] && cmd[i + 1] != ' '
			&& cmd[i + 1] != '|' && cmd[i + 1] != '&')
			mem++;
		i++;
		data->flags->f_esc = false;
	}
	if (!data->flags->f_dquote && !data->flags->f_squote)
	{
		ft_putendl_fd("### allocated argv", 2);
		data->argv = ft_calloc(mem + 2, (sizeof(char *)));
		return (true);
	}
	else
		return (printf(E_NC_QUOTE), false);
}

void	setup_qflags(t_data *data, char *cmd, int *i)
{
	if (!data->flags->f_esc)
	{
		if (cmd[*i] == '\"' && !data->flags->f_squote)
			data->flags->f_dquote = !data->flags->f_dquote;
		if (cmd[*i] == '\'' && !data->flags->f_dquote)
			data->flags->f_squote = !data->flags->f_squote;
	}
}

bool	setup_all_filenames(t_data *data, int *i, char *cmd, int flag)
{
	size_t	start;

	start = *i;
	if (start > ft_strlen(cmd))
	{
		(*i) = ft_strlen(cmd);
		write(2, "Syntax error\n", 14);
		data->exit_status = 2;
		return (false);
	}
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (!flag)
	{
		data->file_name = ft_substr(cmd, start, *i - start);
		printf("filename %s\n", data->file_name);
		// if (access((data->file_name), F_OK) == -1)
		// {
		// 	(*i) = ft_strlen(cmd);
		// 	builtin_error(data, 7, "test", data->exit_status);
		// 	free_array(data->argv);
		// 	return (false);
		// }
	}
	else if (flag == 1)
		data->file_name2 = ft_substr(cmd, start, *i - start);
	else if (flag == 2)
		data->file_name_append = ft_substr(cmd, start, *i - start);

	// ft_putstr_fd("cmd:", 2);
	// ft_putendl_fd(cmd + (*i), 2);
	if (!cmd[*i])
	{
		printf("end of cmd\n");
		return (true);
	}
	printf("not end of cmd i = %d\n", *i);
	return (false);
}

void	remove_quotes_init(t_data *data)
{
	data->rmq.start = 0;
	data->rmq.end = 0;
	data->rmq.f_rmq = false;
}
