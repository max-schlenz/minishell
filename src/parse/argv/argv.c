/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/14 14:06:35 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	alloc_mem_array(t_data *data, char *cmd)
{
	int		wc;
	int		i;
	bool	f_dquote;
	bool	f_squote;
	bool	f_esc;

	wc = 1;
	i = 0;
	f_dquote = false;
	f_squote = false;
	f_esc = false;
	while (cmd[i])
	{
		if (cmd[i] == '\\')
		{
			i++;
			f_esc = true;
		}
		if (cmd[i] == '\"' && !f_squote && !f_esc)
			f_dquote = !f_dquote;
		if (cmd[i] == '\'' && !f_dquote && !f_esc)
			f_squote = !f_squote;
		if (cmd[i] == ' ' && cmd[i + 1]	&& cmd[i + 1] != ' '
			&& cmd[i + 1] != '|' && cmd[i + 1] != '&' && !f_dquote && !f_squote)
			wc++;
		i++;
		f_esc = false;
	}
	if (!f_dquote && !f_squote)
	{
		data->argv = ft_calloc(wc + 1, (sizeof(char *)));
		return (true);
	}
	else
		printf("%s\n", E_NC_QUOTE);
	return (false);
}

bool	split_quotes(t_data *data, char *cmd, int *i)
{
	int		start_args;
	bool	f_dquote;
	bool	f_squote;
	bool	f_esc;
	bool	heredoc_begin;
	int		tmp_fd;

	f_dquote = false;
	f_squote = false;
	f_esc = false;
	heredoc_begin = false;
	start_args = 0;
	tmp_fd = 0;
	if (alloc_mem_array(data, cmd))
	{
		data->parser.arg_start = (*i);
		start_args = (*i);
		data->parser.array_index = 0;
		while (cmd[*i])
		{
			if (!ft_strncmp(cmd + (*i), "&&", 2) && !f_dquote && !f_squote)
				return (parse_and(data, cmd, i, start_args));
			if (!ft_strncmp(cmd + (*i), "||", 2) && !f_dquote && !f_squote)
				return (parse_or(data, cmd, i, start_args));
			if ((cmd[*i] == '>' || cmd[*i] == '<') && !f_dquote && !f_squote)
			{
				if (!ft_strncmp(cmd + (*i), ">>", 2) && !f_dquote && !f_squote)
					return (parse_redir_out(data, cmd, i));
				else if (cmd[*i] == '>')
					data->flags->redir_out = true;
				else if (cmd[*i] == '<')
					data->flags->redir_in = true;
				(*i) += 2;
				data->argv[data->parser.array_index] = NULL;
				if (!data->flags->redir_out)
				{
					if (!set_filenames(data, i, cmd, 0))
						return (true);
				}
				else
				{
					if (!set_filenames(data, i, cmd, 1))
						return (true);
				}		
				(*i)++;
				if (cmd[*i] && cmd[*i] == '|')
					data->flags->pipe = true;
				if (cmd[*i] && cmd[*i] == '>')
				{
					// data->flags->redir_in = false;
					data->flags->redir_out = false;
					data->flags->redir_append = false;
					if (data->file_name2)
					{
						tmp_fd = open(data->file_name2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
						close(tmp_fd);
					}
					continue ;
				}
				return (true);
			}
			if (cmd[*i] == '|' && !f_dquote && !f_squote)
				return (parse_pipes(data, i));
			if (cmd[*i] == '\\')
				f_esc = true;
			if (cmd[*i] == '\"' && !f_squote && !f_esc)
				f_dquote = !f_dquote;
			if (cmd[*i] == '\'' && !f_dquote && !f_esc)
				f_squote = !f_squote;
			if (cmd[*i] == ';' && !f_dquote && !f_esc)
			{
				parse_string(data, cmd, (*i), false);
				(*i) += 1;
				return (true);
			}
			if (cmd[*i] == ' ' && cmd[(*i) + 1] && cmd[(*i) + 1] != ' ' && !f_dquote && !f_squote)
				parse_string(data, cmd, (*i), false);
			(*i)++;
			f_esc = false;
		}
		if (cmd[*i] || data->flags->heredoc || !data->flags->heredoc || heredoc_begin)
		{
			parse_string(data, cmd, (*i), true);
			data->argv[data->parser.array_index] = NULL;
		}
		else
			data->argv[data->parser.array_index] = NULL;
		return (true);
	}
	return (false);
}