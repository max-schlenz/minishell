/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 07:55:07 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/28 12:44:29 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	export_subshell_free(t_data *data)
{
	if (data->subshell.buf)
		free_null (1, &data->subshell.buf);
	if (data->subshell.vcontent)
		free_null (1, &data->subshell.vcontent);
	if (data->subshell.vname)
		free_null (1, &data->subshell.vname);
	if (data->subshell.vname_eq)
		free_null (1, &data->subshell.vname_eq);
	if (data->subshell.var_tmp)
		free_null (1, &data->subshell.var_tmp);
	if (data->subshell.var)
		free_null (1, &data->subshell.var);
}

static char	*export_subshell_join_var(t_data *data, char *setv)
{
	data->subshell.vcontent[ft_strlen(data->subshell.vcontent) - 1] = '\0';
	data->subshell.vname = ft_substr(setv, 0, strlen_var(setv));
	data->subshell.vname_eq = ft_strjoin(data->subshell.vname, "=\"");
	data->subshell.var_tmp
		= ft_strjoin(data->subshell.vname_eq, data->subshell.vcontent);
	return (ft_strjoin(data->subshell.var_tmp, "\""));
}

static char	*export_subshell_store_stdout(t_data *data, char *setv)
{
	char	*tmp;

	tmp = NULL;
	data->subshell.buf = ft_strdup("42");
	while (data->subshell.buf)
	{
		free(data->subshell.buf);
		data->subshell.buf = get_next_line(data->subshell.pipe[0]);
		if (!data->subshell.buf)
			break ;
		if (data->subshell.buf[ft_strlen(data->subshell.buf) - 1] == '\n')
			data->subshell.buf[ft_strlen(data->subshell.buf) - 1] = ' ';
		data->subshell.buf[ft_strlen(data->subshell.buf)] = '\0';
		if (data->subshell.vcontent)
			tmp = data->subshell.vcontent;
		data->subshell.vcontent
			= ft_strjoin_dup(data->subshell.vcontent, data->subshell.buf);
		if (tmp)
			free_null(1, tmp);
	}
	if (!data->subshell.vcontent)
		return (NULL);
	return (export_subshell_join_var(data, setv));
}

static void	export_subshell_fork(t_data *data, char *setv)
{
	t_data	*subsh_data;

	subsh_data = allocate_mem();
	init_vars(subsh_data);
	init_prompt(subsh_data);
	parse_envp(subsh_data, data->envp);
	data->exit_status = prompt(subsh_data, setv + strlen_var(setv) + 2, true);
	clear_buffers(subsh_data);
	cleanup(subsh_data, 0);
}

bool	export_subshell(t_data *data, char *setv)
{
	int		fd_tmp;
	pid_t	pid;
	char	*var;

	if (ft_strlen(setv) > 3 && !ft_strncmp(setv + strlen_var(setv), "=$(", 3))
	{
		export_subshell_init(data);
		fd_tmp = dup(STDOUT_FILENO);
		pipe(data->subshell.pipe);
		dup2(data->subshell.pipe[1], STDOUT_FILENO);
		pid = fork();
		if (!pid)
			export_subshell_fork(data, setv);
		waitpid(-1, NULL, 0);
		close(data->subshell.pipe[1]);
		dup2(fd_tmp, STDOUT_FILENO);
		close (fd_tmp);
		var = export_subshell_store_stdout(data, setv);
		builtin_export(data, var);
		export_subshell_free(data);
		return (data->export.free_set = true, free(var), true);
	}
	return (false);
}
