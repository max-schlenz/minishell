/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/27 13:47:40 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// static int	alloc_argv_quotes(char *cmd, int *i, int *argc, bool reset)
// {
// 	static bool	f_dquote;
// 	static bool	f_squote;

// 	if (reset)
// 	{
// 		f_dquote = false;
// 		f_squote = false;
// 		return (1);
// 	}
// 	if (cmd[(*i)] == '\\')
// 		(*i)++;
// 	else if (cmd[(*i)] == '\"' && !f_squote)
// 		f_dquote = !f_dquote;
// 	else if (cmd[(*i)] == '\'' && !f_dquote)
// 		f_squote = !f_squote;
// 	if (cmd[(*i)] == ' ' && cmd[(*i) + 1] && cmd[(*i) + 1] != ' '
// 		&& cmd[(*i) + 1] != '|' && cmd[(*i) + 1] != '&' && !f_dquote && !f_squote)
// 		(*argc)++;
// 	(*i)++;
// 	if (f_dquote || f_squote)
// 		return (-1);
// 	return (1);
// }

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
		if (cmd[i] == ' ' 
		&& cmd[i + 1] 
		&& cmd[i + 1] != ' ' 
		&& cmd[i + 1] != '|' 
		&& cmd[i + 1] != '&' 
		&& !f_dquote 
		&& !f_squote)
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

char	*get_var_content(t_data *data, char *var)
{
	int		i;
	int		len_var;

	i = 0;
	if (*var == '~')
		var = "$HOME";
	var++;
	len_var = ft_strlen(var);
	if (!len_var)
		return (NULL);
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], var, len_var))
			return (ft_strdup(data->envp[i] + len_var + 1));
		i++;
	}
	return (ft_strdup(""));
}

static char	*rm_quotes_start(t_data *data, int *i, int i_arg, char *tmp)
{
	char	*str_before_q;
	char	*str_wo_q;
	char	*ret;
	char	delim;

	delim = data->argv[i_arg][(*i)];
	if (!tmp && (*i) > 0)
		str_before_q = ft_substr(data->argv[i_arg], 0, (*i));
	else
		str_before_q = tmp;
	while (data->argv[i_arg][(*i)] && data->argv[i_arg][(*i)] == delim)
		(*i)++;
	data->rmq.start = (*i);
	if (data->argv[i_arg][(*i)] && data->argv[i_arg][(*i) + 1])
		(*i)++;
	while (data->argv[i_arg][(*i)] && data->argv[i_arg][(*i)] != delim)
		(*i)++;
	data->rmq.end = (*i);
	if (data->rmq.start != data->rmq.end)
		str_wo_q = ft_substr(data->argv[i_arg], data->rmq.start, data->rmq.end - data->rmq.start);
	else
		str_wo_q = ft_strdup("");
	ret = ft_strjoin_dup(str_before_q, str_wo_q);
	free (str_wo_q);
	free (str_before_q);
	return (ret);
}

static void	rm_quotes_wr_argv(t_data *data, int i_arg, char *tmp)
{
	char *argv;

	if (ft_strlen(data->argv[i_arg]) >= data->rmq.end + 1)
		argv = ft_strdup(data->argv[i_arg] + data->rmq.end + 1);
	else
		argv = ft_strdup("");
	free(data->argv[i_arg]);
	data->argv[i_arg] = NULL;
	data->argv[i_arg] = ft_strjoin_dup(tmp, argv);
	free (tmp);
	free (argv);
}

static char	*rm_quotes_mid(t_data *data, int *i, int i_arg, char *argv)
{
	char	*str_between;
	char	*ret;
	char	*tmp;
	
	str_between = ft_substr(data->argv[i_arg], (*i), 1);
	data->rmq.end++;
	tmp = argv;
	ret = ft_strjoin_dup(argv, str_between);
	free (tmp);
	free (str_between);
	return (ret);
}

static void remove_quotes(t_data *data, int i_arg)
{
	char	*argv;
	bool	f_rmq;
	int 	i;

	f_rmq = false;
	argv = NULL;
	data->rmq.start = 0;
	data->rmq.end = 0;
	i = 0;
	while (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
	{
		if (data->argv[i_arg][i] == '\\')
			i++;
		else if (!data->rmq.start && (data->argv[i_arg][i] == '\"' || data->argv[i_arg][i] == '\''))
		{
			f_rmq = true;
			argv = rm_quotes_start(data, &i, i_arg, argv);
		}
		else if (f_rmq)
			argv = rm_quotes_mid(data, &i, i_arg, argv);
		data->rmq.start = 0;
		i++;
	}
	if (argv)
		rm_quotes_wr_argv(data, i_arg, argv);
}

static void	remove_backslashes(t_data *data, int i_arg)
{
	int		i;

	i = 0;
	while (data->argv[i_arg][i])
	{
		if (data->argv[i_arg][i] == '\\' && data->argv[i_arg][i + 1] == ' ')
			data->argv[i_arg][i] = ' ';
		else if (data->argv[i_arg][i] == '\\')
		{
			while (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
			{
				data->argv[i_arg][i] = data->argv[i_arg][i + 1];
				i++;
			}
			data->argv[i_arg][i] = '\0';
		}
		i++;
	}
}

static bool	check_var_exists(t_data *data, char *var)
{
	int i;
	char *tmp;

	i = 0;
	if ((var && (var[0] == '~' || !ft_strncmp(var, "$?", 2))) || (var && var[0] && var[1] && isnumeric(var[1])))
		return (true);
	tmp = ft_strjoin(var + 1, "=");
	while (data->envp[i])
	{
		if (!ft_strncmp(tmp, data->envp[i], ft_strlen(tmp)))
		{
			free(tmp);
			return (true);
		}
		i++;
	}
	free(tmp);
	return (false);
}

// static void	expand_exit_status(t_data *data)
// {
	
// }

bool	expand_vars(t_data *data)
{
	size_t	i_char;
	int		i_arg;
	char	*str_before_v;
	char	*vname;
	char	*vcontent;
	char	*str_before_vplus_vcontent;
	char	*str_after_v;
	bool	f_dquote;
	bool	f_squote;
	bool	f_esc;

	i_arg = 0;
	i_char = 0;
	vname = NULL;
	while (data->argv[i_arg])
	{
		f_dquote = false;
		f_squote = false;
		f_esc = false;
		while (data->argv[i_arg][i_char])
		{
			if (data->argv[i_arg][i_char] == '\"')
				f_dquote = !f_dquote;
			if (data->argv[i_arg][i_char] == '\'' && !f_dquote)
				f_squote = !f_squote;
			if (data->argv[i_arg][i_char] == '\\')
			{
				f_esc = true;
				i_char++;
				continue ;
			}
			if ((!data->flags->noenv
			&& data->argv[i_arg][0] == '~' 
			&& (!ft_isalnum(data->argv[i_arg][1])
			&& !f_dquote && !f_squote && !f_esc))
			|| (data->argv[i_arg][i_char] == '$' 
			&& (data->argv[i_arg][i_char + 1]) 
			&& (data->argv[i_arg][i_char + 1] != ' ') 
			&& (!f_squote && !f_esc)))
			{
				str_before_v = ft_substr(data->argv[i_arg], 0, i_char);
				vname = ft_substr(data->argv[i_arg], i_char, strlen_path(data->argv[i_arg] + i_char));
				if (!check_var_exists(data, vname) && data->argv[i_arg][i_char] && data->argv[i_arg][i_char + 1] && ft_strncmp(data->argv[i_arg] + i_char, "$?", 2))
				{
					str_after_v = ft_strdup(data->argv[i_arg] + i_char + ft_strlen(vname));
					free(data->argv[i_arg]);
					if (ft_strlen(str_before_v) > 0)
						data->argv[i_arg] = ft_strjoin(str_before_v, str_after_v);
					else
						data->argv[i_arg] = ft_strdup("");
					free (str_before_v);
					str_before_v = NULL;
					free (str_after_v);
					str_after_v = NULL;
					free (vname);
					vname = NULL;
					continue ;
				}
				if (data->argv[i_arg][i_char] && data->argv[i_arg][i_char + 1] && !ft_strncmp(data->argv[i_arg] + i_char, "$?", 2))
				{
					i_char++;
					vcontent = ft_itoa(data->exit_status);
				}
				else if (data->argv[i_arg][i_char + 1] && isnumeric(data->argv[i_arg][i_char + 1]))
				{
					i_char++;
					free (vname);
					vname = ft_substr(data->argv[i_arg], i_char, 1);
					if (data->argv[i_arg][i_char] == '0')
						vcontent = ft_strdup("minishell");
					else
						vcontent = ft_strdup("");
				}
				else
					vcontent = get_var_content(data, vname);
				if (vcontent)
				{
					str_before_vplus_vcontent = ft_strjoin(str_before_v, vcontent);
					str_after_v = ft_strdup(data->argv[i_arg] + i_char + ft_strlen(vname));
					free (data->argv[i_arg]);
					if (ft_strlen(str_before_vplus_vcontent) > 0 || ft_strlen(str_after_v) > 0)
						data->argv[i_arg] = ft_strjoin(str_before_vplus_vcontent, str_after_v);
					else
						data->argv[i_arg] = ft_strdup("");
					free (str_before_vplus_vcontent);
					free (str_after_v);
					free (vcontent);
					vcontent = NULL;
				}
				free (str_before_v);
				free (vname);
				vname = NULL;
				if (i_char > ft_strlen(data->argv[i_arg]))
					break ;
				continue ;
			}
			if (data->argv[i_arg][i_char] && data->argv[i_arg][i_char + 1])
				i_char++;
			else 
				break ;
			f_esc = false;
		}
		if (!data->argv[i_arg])
			break ;
		if (ft_strlen(data->argv[i_arg]) > 2)
		{
			remove_quotes(data, i_arg);
			remove_backslashes(data, i_arg);
		}
		else if (ft_strncmp(data->argv[0], "echo", 4) 
			&& (!ft_strncmp(data->argv[i_arg], "\'\'", 2) 
			|| !ft_strncmp(data->argv[i_arg], "\"\"", 2)))
		{
			free(data->argv[i_arg]);
			data->argv[i_arg] = ft_strdup("");
		}
		if (!data->argv[i_arg][0])
		{
			if (data->argc == i_arg)
			{
				free (data->argv[i_arg]);
				data->argv[i_arg] = NULL;
			}
		}
		i_char = 0;
		i_arg++;
	}
	return (true);
}

bool	set_filenames(t_data *data, int *i, char *cmd, int flag)
{
	size_t	start;

	start = *i;
	if (start >= ft_strlen(cmd))
	{
		write(2, "Syntax error\n", 14);
		data->exit_status = 2;
		return(false);
	}
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (!flag)
		data->file_name = ft_substr(cmd, start, *i - start);
	else
		data->file_name2 = ft_substr(cmd, start, *i - start);
	if (!cmd[*i])
		return (false);
	return (true);
}

bool	heredoc_delim(t_data *data, int *i, int *j, char *cmd)
{
	int	start;
	char *tmp;

	start = *i;
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	tmp = ft_substr(cmd, start, *i - start);
	data->heredoc_delim = ft_strjoin(tmp, "\n");
	free (tmp);
	(*j) += ft_strlen(data->heredoc_delim);
	if (!cmd[*i])
		return (false);
	return (true);
}

static void	parse_string(t_data *data, char *cmd, int *array_index, int i, int j)
{
	free(data->argv[*array_index]);
	data->argv[*array_index] = NULL;
	data->argv[*array_index] = ft_substr(cmd, j, i - j);
	data->argc = *array_index;
	return ;
}

bool	split_quotes(t_data *data, char *cmd, int *i)
{
	int		j;
	int		k;
	int		array_index;
	bool	f_dquote;
	bool	f_squote;
	bool	f_esc;
	bool	heredoc_begin;
	int		tmp_fd;

	f_dquote = false;
	f_squote = false;
	f_esc = false;
	heredoc_begin = false;
	k = 0;
	tmp_fd = 0;
	if (alloc_mem_array(data, cmd))
	{
		j = (*i);
		k = (*i);
		array_index = 0;
		while (cmd[*i])
		{
			if (!ft_strncmp(cmd + (*i), "&&", 2) && !f_dquote && !f_squote)
			{
				if ((*i) != k)
					return (true);
				data->flags->and = true;
				data->flags->or = false;
				data->flags->pipe = false;
				(*i) += 3;
				data->fd_i = 0;
				count_pipes(data, cmd + (*i));
				wait_for_childs(data);
				// if (data->flags->heredoc)
				// 	continue ;
				return (true);
			}
			if (!ft_strncmp(cmd + (*i), "||", 2) && !f_dquote && !f_squote)
			{
				if ((*i) != k)
					return (true);
				data->flags->and = false;
				data->flags->or = true;
				(*i) += 3;
				data->fd_i = 0;
				count_pipes(data, cmd + (*i));
				wait_for_childs(data);
				return (true);
			}
			if (!ft_strncmp(cmd + (*i), "<<", 2) && !f_dquote && !f_squote)
			{
				if ((*i) != 0)
				{
					(*i) += 3;
					j = (*i);
					data->flags->heredoc = true;
					data->argv[array_index] = NULL;
					if (!heredoc_delim(data, i, &j, cmd))
						return (false);
					(*i)++;
				}
				else
				{
					heredoc_begin = true;
					(*i) += 3;
					j = (*i);
					data->flags->heredoc = true;
					heredoc_delim(data, i, &j, cmd);
					(*i) = ft_strlen(data->heredoc_delim) + 3;
				}
			}
			if ((cmd[*i] == '>' || cmd[*i] == '<') && !f_dquote && !f_squote)
			{
				if (!ft_strncmp(cmd + (*i), ">>", 2) && !f_dquote && !f_squote)
				{
					(*i)++;
					data->flags->redir_append = true;
				}
				else if (cmd[*i] == '>')
					data->flags->redir_out = true;
				else if (cmd[*i] == '<')
					data->flags->redir_in = true;
				(*i) += 2;
				data->argv[array_index] = NULL;
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
					data->flags->redir_in = false;
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
			{
				data->flags->pipe = true;
				(*i) += 2;
				return (true);
			}
			if (cmd[*i] == '\\')
				f_esc = true;
			if (cmd[*i] == '\"' && !f_squote && !f_esc)
				f_dquote = !f_dquote;
			if (cmd[*i] == '\'' && !f_dquote && !f_esc)
				f_squote = !f_squote;
			if (cmd[*i] == ';' && !f_dquote && !f_esc)
			{
				parse_string(data, cmd, &array_index, (*i), j);
				array_index++;
				j = (*i) + 1;
				(*i) += 1;
				return (true);
			}
			if (cmd[*i] == ' ' && cmd[(*i) + 1] && cmd[(*i) + 1] != ' ' && !f_dquote && !f_squote)
			{
				parse_string(data, cmd, &array_index, (*i), j);
				array_index++;
				j = (*i) + 1;
			}
			(*i)++;
			f_esc = false;
		}
		if (cmd[*i] || data->flags->heredoc || !data->flags->heredoc || heredoc_begin)
		{
			parse_string(data, cmd, &array_index, (*i), j);
			data->argv[++array_index] = NULL;
		}
		else
			data->argv[array_index] = NULL;
		return (true);
	}
	return (false);
}
