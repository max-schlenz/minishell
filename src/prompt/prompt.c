/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/11 12:47:50 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// data->cmd = get_next_line(0);
// data->cmd = ft_strtrim(data->cmd, "\n");
//	builds and displays the prompt
static void	show_prompt(t_data *data)
{
	char	*cwd;
	char	*prompt_cwd;
	char	*prompt;
	int		i;

	i = 0;
	cwd = getcwd(NULL, 0);
	while (cwd[ft_strlen(cwd) - i] != '/')
		i++;
	prompt_cwd = ft_strjoin(data->prompt, cwd + ft_strlen(cwd) - i + 1);
	prompt = ft_strjoin(prompt_cwd, PROMPT_SUFFIX);
	free_null (2, &prompt_cwd, &cwd);
	data->cmd = readline(prompt);
	free (prompt);
}

//	pre-parsing of the command
static bool	prompt_prep(t_data *data, char **tmp_cmd)
{
	history(data);
	*tmp_cmd = pre_parse(data, data->cmd);
	if (!(*tmp_cmd))
		return (false);
	if (prompt_syntax_check(data, tmp_cmd))
	{
		*tmp_cmd = handle_heredoc(data, *tmp_cmd);
		return (true);
	}
	return (false);
}

void	dbg(t_data *data)
{
	int	i;

	i = 0;
	data->debug = fopen(".debug", "a+");
	fprintf(data->debug, "\n------------------\n");
	while (data->argv[i])
	{
		fprintf(data->debug, "argv[%d] = %s\n", i, data->argv[i]);
		i++;
	}
	fprintf(data->debug, "\nFLAGS:\n");
	fprintf(data->debug, "redir_out    : %d\n", data->flags->redir_out);
	fprintf(data->debug, "redir_in     : %d\n", data->flags->redir_in);
	fprintf(data->debug, "heredoc      : %d\n", data->flags->heredoc);
	fprintf(data->debug, "redir_append : %d\n", data->flags->redir_append);
	fprintf(data->debug, "pipe         : %d\n\n", data->flags->pipe);
	fprintf(data->debug, "and          : %d\n", data->flags->and);
	fprintf(data->debug, "or           : %d\n", data->flags->or);
	fprintf(data->debug, "data->fd_i            : %d\n", data->fd_i);
	fprintf(data->debug, "data->counter_pipes   : %d\n", data->counter_pipes);
	fprintf(data->debug, "data->file_name       : %s\n", data->file_name);
	fprintf(data->debug, "data->file_name2      : %s\n", data->file_name2);
	fprintf(data->debug, "data->file_name_append: %s\n", data->file_name_append);
	fprintf(data->debug, "data->exit_status (p) : %lld\n", data->exit_status);
	fclose(data->debug);
}

//	evaluates if execution should take place and calls executor
static void	prompt_exec(t_data *data)
{
	dbg(data);
	if (data->flags->prompt_exec
		|| (!data->flags->prompt_exec
			&& ((!data->flags->and && !data->flags->or)
				|| (data->flags->and && !data->exit_status)
				|| (data->flags->or && data->exit_status))))
	{
		if (!builtin(data))
			exec_program(data);
	}
	data->flags->prompt_exec = false;
	free_array(data->argv);
	data->argv = NULL;
}

//	parsing of the command, expanding variables and preparing for execution
static void	prompt_iter(t_data *data, char *tmp_cmd)
{
	int		i;

	i = 0;
	while (tmp_cmd[i] && tmp_cmd[0])
	{
		while (tmp_cmd[i] == ' ' || tmp_cmd[i] == ';'
			|| (tmp_cmd[i] && tmp_cmd[i + 1]
				&& tmp_cmd[i] == '&' && tmp_cmd[i + 1] != '&'))
					i++;
		setup_argv(data, tmp_cmd, &i);
		if (!i)
		{
			free_array(data->argv);
			break ;
		}
		expand_vars(data);
		resolve_wildcards(data);
		priorities(data, &tmp_cmd, &i);
		if (!data->argv[0])
		{
			free_array(data->argv);
			continue ;
		}
		prompt_exec(data);
	}
}

//	displays a prompt and waits for user input, or - if given - processes 'cmd'.
int	prompt(t_data *data, char *cmd, int flag)
{
	char	*tmp_cmd;

	data->flags->prompt_exec = true;
	data->cmd = NULL;
	tmp_cmd = NULL;
	if (flag)
		data->cmd = ft_strdup(cmd);
	else
		show_prompt(data);
	if (!data->cmd)
		data->cmd = ft_strdup("exit");
	if (data->cmd[0] && data->cmd[0] != '\n')
	{
		if (!prompt_prep(data, &tmp_cmd))
			return (data->exit_status);
	}
	else
		return (0);
	prompt_iter(data, tmp_cmd);
	exec_wait_for_childs(data);
	signals(false);
	free_null(1, &tmp_cmd);
	return (data->exit_status);
}
