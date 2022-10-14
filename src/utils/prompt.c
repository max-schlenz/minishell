/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 23:02:37 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	show_prompt(t_data *data)
{
	char	*cwd;
	char	*prompt_cwd;
	char	*prompt;
	int		i;
	// data->cmd = get_next_line(0);
	// data->cmd = ft_strtrim(data->cmd, "\n");
	i = 0;
	cwd = getcwd(NULL, 0);
	while (cwd[ft_strlen(cwd) - i] != '/')
		i++;
	prompt_cwd = ft_strjoin(data->prompt, cwd + ft_strlen(cwd) - i + 1);
	free (cwd);
	prompt = ft_strjoin(prompt_cwd, PROMPT_SUFFIX);
	free (prompt_cwd);
	data->cmd = readline(prompt);
	free (prompt);
}

int prompt(t_data *data, char *cmd, int flag)
{
	bool	left;
	char	*tmp_cmd;
	int		i;

	left = true;
	data->cmd = NULL;
	if (flag)
		data->cmd = ft_strdup(cmd);
	else
		show_prompt(data);
	i = 0;
	if (!data->cmd)
		data->cmd = ft_strdup("exit");
	if (data->cmd[0] && data->cmd[0] != '\n')
	{
		history(data);
		tmp_cmd = pre_parse(data, data->cmd);
		if (!check_syntax(data, tmp_cmd)
			|| !syntax_err(data, tmp_cmd)
			|| !check_syntax_first_char(data, tmp_cmd))
		{
			free(tmp_cmd);
			tmp_cmd = NULL;
			return (data->exit_status);
		}
		count_pipes(data, tmp_cmd);
	}
	else
		return (0);
	tmp_cmd = handle_heredoc(data, tmp_cmd);
	while (tmp_cmd[i] && tmp_cmd[0])
	{
		while (tmp_cmd[i] == ' ' || tmp_cmd[i] == ';')
			i++;
		split_quotes(data, tmp_cmd, &i);
		if (!tmp_cmd[i - 1])
		{
			free_array(data->argv);
			free(data->argv);
			break ;
		}
		expand_vars(data);
		get_all_names(data);
		if (data->argv[0] && (data->argv[0][0] == '(' || data->flags->bracket))
			prio(data, tmp_cmd, &i);
		if (!data->argv[0])
		{
			free_array(data->argv);
			free(data->argv);
			continue ;
		}
		if ((left)
			|| (!data->flags->and && !data->flags->or)
			|| (data->flags->and && !data->exit_status)
			|| (data->flags->or && data->exit_status))
		{
			if (data->flags->debug)
			{
				data->debug = fopen(DBG, "a+");
				dbg(data);
			}
			if (!builtin_environment(data))
				exec_program(data);
			if (data->flags->debug && data->debug)
				fclose(data->debug);
		}
		left = false;
		free_array(data->argv);
		free(data->argv);
	}
	wait_for_childs(data);
	signals(false);
	free(tmp_cmd);
	tmp_cmd = NULL;
	return (data->exit_status);
}