/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/20 10:45:22 by mschlenz         ###   ########.fr       */
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
	int i;
	int	len_var;

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
			return(ft_strdup(data->envp[i] + len_var + 1));
		i++;
	}
	return (ft_strdup(""));
}

static void remove_quotes(t_data *data, int i_arg)
{
	char *tmp = NULL;
	char *tmp2 = NULL;
	char *tmp3 = NULL;
	char *tmp4 = NULL;
	char *str_between = NULL;
	char *tmp_esc = NULL;
	char *tmp_esc2 = NULL;
	int	inbetween = 0;
	bool	f_esc = false;
	bool	f_rmq = false;
	int i = 0;
	char delim;
	int j = 0;
	int k = 0;
	int start = 0;
	int end = 0;
	delim = 0;
	// int k = 0;
	// while (data->argv[k])
		// printf("%s\n", data->argv[k++]);
	// while (data->argv[i_arg][k])
	// {
	// 	if (data->argv[i_arg][k] == '\\')
	// 	{
	// 		f_esc = true;
	// 		tmp_esc2 = ft_substr(data->argv[i_arg], 0, i);
	// 		printf("%s\n", tmp_esc2);
	// 		i++;
	// 	}
	// 	k++;
	// }
	while (data->argv[i_arg][i])
	{
		if (data->argv[i_arg][i] == '\\')
		{
			f_esc = true;
			i++;
		}
		if (!f_esc && !start && (data->argv[i_arg][i] == '\"' || data->argv[i_arg][i] == '\''))
		{
			f_rmq = true;
			delim = data->argv[i_arg][i];
			if (!tmp && i > 0)
				tmp = ft_substr(data->argv[i_arg], 0, i);
			while (data->argv[i_arg][i] && data->argv[i_arg][i] == delim)
				i++;
			start = i;
			if (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
				i++;
			while (data->argv[i_arg][i] && data->argv[i_arg][i] != delim)
				i++;
			end = i;
			if (start != end)
				tmp2 = ft_substr(data->argv[i_arg], start, end - start);
			else
				tmp2 = ft_strdup("");
			tmp4 = tmp;
			tmp = ft_strjoin_dup(tmp, tmp2);
			free (tmp4);
			free (tmp2);
		}
		else if (f_rmq)
		{
			str_between = ft_substr(data->argv[i_arg], i, 1);
			end++;
			tmp4 = tmp;
			tmp = ft_strjoin_dup(tmp, str_between);
			free (tmp4);
			free (str_between);
		}
		start = 0;
		f_esc = false;
		if (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
			i++;
		else 
			break ;
	}
	if (tmp)
	{
		// printf("%d\n", inbetween);
		if (ft_strlen(data->argv[i_arg]) >= end + 1)
			tmp3 = ft_strdup(data->argv[i_arg] + end + 1);
		else
			tmp3 = ft_strdup("");
		free(data->argv[i_arg]);
		data->argv[i_arg] = NULL;
		data->argv[i_arg] = ft_strjoin_dup(tmp, tmp3);
		free (tmp);
		free (tmp3);
	}
}

// static void	remove_backslash(t_data *data, int i_arg)
// {
// 	int		i;

// 	i = 0;
// 	while (data->argv[i_arg][i])
// 	{
// 		if (data->argv[i_arg][i] == '\\')
// 			data->argv[i_arg][i] = ' ';
// 		i++;
// 	}
// }

bool	expand_vars(t_data *data)
{
	int i_arg;
	int	i_char;
	int	i = 0;

	char *str_before_v;
	char *vname;
	char *vcontent;
	char *str_before_vplus_vcontent;
	char *str_after_v;
	bool f_dquote;
	bool f_squote;
	bool f_esc;
	bool reset = false;

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
			if (data->argv[i_arg][0] == '~' 
			&& (!ft_isalnum(data->argv[i_arg][1])
			&& !f_dquote && !f_squote && !f_esc) 
			|| (data->argv[i_arg][i_char] == '$' 
			&& (data->argv[i_arg][i_char + 1]) 
			&& (data->argv[i_arg][i_char + 1] != ' ') 
			&& (!f_squote && !f_esc)))
			{
				str_before_v = ft_substr(data->argv[i_arg], 0, i_char);
				vname = ft_substr(data->argv[i_arg], i_char, strlen_path(data->argv[i_arg] + i_char));
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
					vcontent = ft_strdup("");
				}
				else
					vcontent = get_var_content(data, vname);
				if (vcontent)
				{
					str_before_vplus_vcontent = ft_strjoin(str_before_v, vcontent);
					str_after_v = ft_strdup(data->argv[i_arg] + i_char + ft_strlen(vname));
					free (data->argv[i_arg]);
					data->argv[i_arg] = ft_strjoin(str_before_vplus_vcontent, str_after_v);
					free (str_before_vplus_vcontent);
					free (str_after_v);
					free (vcontent);
					vcontent = NULL;
				}
				free (str_before_v);
				free (vname);
				vname = NULL;
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
			remove_quotes(data, i_arg);
		// else if (data->argv[i_arg][0] == '\'' || data->argv[i_arg][0] == '\"')
		else if (!ft_strncmp(data->argv[i_arg], "\'\'", 2) || !ft_strncmp(data->argv[i_arg], "\"\"", 2))
		{
			free(data->argv[i_arg]);
			data->argv[i_arg] = ft_strdup(" ");
		}
		if (!data->argv[i_arg][0])
		{
			free(data->argv[i_arg]);
			if (data->argc == i_arg)
				data->argv[i_arg] = NULL;
			else
				data->argv[i_arg] = ft_strdup("");
		}
		i_char = 0;
		i_arg++;
	}
	return (true);
}

bool	set_filenames(t_data *data, int *i, char *cmd, int flag)
{
	int	start;

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
	int k = 0;

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

	f_dquote = false;
	f_squote = false;
	f_esc = false;
	heredoc_begin = false;
	k = 0;
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
				close_pipes(data);
				data->fd_i = 0;
				if (count_pipes(data, cmd + (*i)))
					open_pipes(data);
				return (true);
			}
			if (!ft_strncmp(cmd + (*i), "||", 2) && !f_dquote && !f_squote)
			{
				if ((*i) != k)
					return (true);
				data->flags->and = false;
				data->flags->or = true;
				(*i) += 3;
				close_pipes(data);
				data->fd_i = 0;
				if (count_pipes(data, cmd + (*i)))
					open_pipes(data);
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
						return (true);
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
					data->flags->redir_out = true;
					if (!set_filenames(data, i, cmd, 1))
						return (true);
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
		if (cmd[*i] || !data->flags->heredoc || heredoc_begin)
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
