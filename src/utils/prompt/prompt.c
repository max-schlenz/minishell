/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/15 20:08:07 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// data->cmd = get_next_line(0);
// data->cmd = ft_strtrim(data->cmd, "\n");
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
	free (cwd);
	prompt = ft_strjoin(prompt_cwd, PROMPT_SUFFIX);
	free (prompt_cwd);
	data->cmd = readline(prompt);
	free (prompt);
}

static bool	prompt_prep(t_data *data, char **tmp_cmd)
{
	history(data);
	*tmp_cmd = pre_parse(data, data->cmd);
	if (!check_syntax(data, *tmp_cmd)
		|| !syntax_err(data, *tmp_cmd)
		|| !check_syntax_first_char(data, *tmp_cmd))
	{
		free(*tmp_cmd);
		*tmp_cmd = NULL;
		return (false);
	}
	count_pipes(data, *tmp_cmd);
	*tmp_cmd = handle_heredoc(data, *tmp_cmd);
	return (true);
}

static void	prompt_exec(t_data *data)
{
	if ((EXEC) || (!AND && !OR) || (AND && !EXIT) || (OR && EXIT))
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
	data->flags->prompt_exec = false;
	free_array(data->argv);
	free(data->argv);
}

static void	prompt_iter(t_data *data, char *tmp_cmd)
{
	int		i;

	i = 0;
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
		priorities(data, &tmp_cmd, &i);
		if (!data->argv[0])
		{
			free_array(data->argv);
			free(data->argv);
			continue ;
		}
		prompt_exec(data);
	}
}

int	prompt(t_data *data, char *cmd, int flag)
{
	char	*tmp_cmd;

	data->flags->prompt_exec = true;
	data->cmd = NULL;
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
	wait_for_childs(data);
	signals(false);
	free_str(1, tmp_cmd);
	return (data->exit_status);
}
