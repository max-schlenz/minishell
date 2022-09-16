/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/16 18:06:32 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	alloc_mem_array(t_data *data, char *cmd)
{
	int		wc;
	int		i;
	bool	f_dquote;
	bool	f_squote;

	wc = 1;
	i = 0;
	f_dquote = false;
	f_squote = false;
	while (cmd[i])
	{
		if (cmd[i] == '\"' && !f_squote)
			f_dquote = !f_dquote;
		if (cmd[i] == '\'' && !f_dquote)
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

static char	*get_var_content(t_data *data, char *var)
{
	int i;
	int	len_var;

	i = 0;
	var++;
	len_var = ft_strlen(var);
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], var, len_var))
			return (ft_substr(data->envp[i], len_var + 1, ft_strlen(data->envp[i])));
		i++;
	}
	return (ft_strdup(""));
}

static void remove_quotes(t_data *data, int i_arg, bool f_dquote, bool f_squote)
{
	char **tmp = NULL;
	int i = 0;
	char delim;

	delim = 0;
	if (!f_squote && f_dquote)
		delim = '\"';
	if (!f_dquote && f_squote)
		delim = '\'';
	if (f_dquote && f_squote)
		delim = '\'';
	if (delim)
	{
		tmp = ft_split(data->argv[i_arg], delim);
		free(data->argv[i_arg]);
		data->argv[i_arg] = NULL;
		while (tmp[i])
		{
			data->argv[i_arg] = ft_strjoin_dup(data->argv[i_arg], tmp[i]);
			free(tmp[i++]);
		}
	}
}

static void	remove_backslash(t_data *data, int i_arg)
{
	int		i;

	i = 0;
	while (data->argv[i_arg][i])
	{
		if (data->argv[i_arg][i] == '\\')
			data->argv[i_arg][i] = ' ';
		i++;
	}
}

// static void	remove_backslash(t_data *data, int i_arg)
// {
// 	char	**tmp;
// 	char	*space;
// 	int		i;

// 	i = 0;
// 	space = ft_strdup("ZZZ");
// 	tmp = ft_split(data->argv[i_arg], '\\');
// 	free(data->argv[i_arg]);
// 	data->argv[i_arg] = NULL;
// 	while (tmp[i])
// 	{
// 		if (!i)
// 			data->argv[i_arg] = ft_strjoin_dup(tmp[i], space);
// 		data->argv[i_arg] = ft_strjoin_dup(data->argv[i_arg], tmp[i]);
// 		free(tmp[i++]);
// 	}
// }

void	expand_vars(t_data *data)
{
	int i_arg;
	int	i_char;

	char *str_before_v;
	char *vname;
	char *vcontent;
	char *str_before_vplus_vcontent;
	char *str_after_v;
	bool f_dquote;
	bool f_squote;
	bool f_backslash;

	i_arg = 0;
	i_char = 0;
	while (data->argv[i_arg])
	{
		f_dquote = false;
		f_squote = false;
		f_backslash = false;
		while (data->argv[i_arg][i_char])
		{
			if (data->argv[i_arg][i_char] == '\"')
				f_dquote = true;
			if (data->argv[i_arg][i_char] == '\'' && !f_dquote)
				f_squote = true;
			if (data->argv[i_arg][i_char] == '\\')
			{
				f_backslash = true;
				i_char++;
			}
			if (data->argv[i_arg][i_char] == '$' && !f_squote && !f_backslash)
			{
				str_before_v = ft_substr(data->argv[i_arg], 0, i_char);
				str_before_v = ft_strtrim(str_before_v, "\"");
				vname = ft_substr(data->argv[i_arg], i_char, strlen_path(data->argv[i_arg] + i_char));
				if (data->argv[i_arg] + i_char && data->argv[i_arg] + i_char + 1 && !ft_strncmp(data->argv[i_arg] + i_char, "$?", 2))
				{
					i_char++;
					vcontent = ft_strdup(ft_itoa(data->exit_status));
				}
				else
					vcontent = get_var_content(data, vname);
				str_before_vplus_vcontent = ft_strjoin(str_before_v, vcontent);
				// str_after_v = ft_substr(data->argv[i_arg], i_char + ft_strlen(vname), ft_strlen(data->argv[i_arg]) - i_char + ft_strlen(vname));
				str_after_v = ft_strdup(data->argv[i_arg] + i_char + ft_strlen(vname));
				str_after_v = ft_strtrim(str_after_v, "\"");
				free (vname);
				free (vcontent);
				free (data->argv[i_arg]);
				data->argv[i_arg] = ft_strjoin(str_before_vplus_vcontent, str_after_v);
				free (str_before_vplus_vcontent);
				free (str_after_v);
			}
			i_char++;
			f_backslash = false;
		}
		if (ft_strlen(data->argv[i_arg]) > 2)
			remove_quotes(data, i_arg, f_dquote, f_squote);
		// if (f_backslash)
		// 	remove_backslash(data, i_arg);
		else if (data->argv[i_arg][0] == '\'' || data->argv[i_arg][0] == '\"')
			data->argv[i_arg] = ft_strdup(" ");
		i_char = 0;
		i_arg++;
	}
	// while (data->argv[k])
	// 	printf("%s\n", data->argv[k++]);
	// exit(0);
}

bool	set_filename2(t_data *data, int *i, char *cmd)
{
	int	start;

	start = *i;
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
	{
		(*i)++;
	}
	data->file_name2 = ft_substr(cmd, start, *i - start);
	if (!cmd[*i])
		return (false);
	return (true);
}

bool	set_filename(t_data *data, int *i, char *cmd)
{
	int	start;

	start = *i;
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '>' && cmd[*i] != '<')
	{
		(*i)++;
	}
	data->file_name = ft_substr(cmd, start, *i - start);
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

void	skip_spaces(char *cmd, int *i)
{
	while (cmd[*i] == ' ')
		(*i)++;
}

static void	parse_string(t_data *data, char *cmd, int array_index, int i, int j)
{
	free(data->argv[array_index]);
	data->argv[array_index] = ft_substr(cmd, j, i - j);
	return ;
}

int	split_quotes(t_data *data, char *cmd, int i)
{
	int		j;
	int		k;
	int		array_index;
	bool	f_dquote;
	bool	f_squote;

	f_dquote = false;
	f_squote = false;
	k = 0;
	if (alloc_mem_array(data, cmd))
	{
		j = i;
		k = i;
		array_index = 0;
		while (cmd[i])
		{
			if (!ft_strncmp(cmd + i, "&&", 2) && !f_dquote && !f_squote)
			{
				// if (i != 0)
				// 	return (i);
				if (i != k)
					return (i);
				data->flags->and = true;
				data->flags->or = false;
				data->flags->pipe = false;
				i += 3;
				close_pipes(data);
				data->fd_i = 0;
				if (count_pipes(data, cmd + i))
					open_pipes(data);
				return (i);
			}
			if (!ft_strncmp(cmd + i, "||", 2) && !f_dquote && !f_squote)
			{
				if (i != k)
					return (i);
				data->flags->and = false;
				data->flags->or = true;
				i += 3;
				close_pipes(data);
				data->fd_i = 0;
				if (count_pipes(data, cmd + i))
					open_pipes(data);
				return (i);
			}
			if (!ft_strncmp(cmd + i, "<<", 2) && !f_dquote && !f_squote)
			{
				i += 3;
				j = i;
				data->flags->heredoc = true;
				data->argv[array_index] = NULL;
				if (!heredoc_delim(data, &i, &j, cmd))
					return (i);
				i++;
			}
			if ((cmd[i] == '>' || cmd[i] == '<') && !f_dquote && !f_squote)
			{
				if (!ft_strncmp(cmd + i, ">>", 2) && !f_dquote && !f_squote)
				{
					i++;
					data->flags->redir_append = true;
				}
				else if (cmd[i] == '>')
					data->flags->redir_out = true;
				else if (cmd[i] == '<')
					data->flags->redir_in = true;
				i += 2;
				data->argv[array_index] = NULL;
				if (!data->flags->redir_out)
				{
					if (!set_filename(data, &i, cmd))
						return (i);
				}
				else
				{
					if (!set_filename2(data, &i, cmd))
						return (i);
				}		
				i++;
				if (cmd[i] && cmd[i] == '|')
					data->flags->pipe = true;
				if (cmd[i] && cmd[i] == '>')
				{
					data->flags->redir_out = true;
					if (!set_filename2(data, &i, cmd))
						return (i);
				}	
				return (i);
			}
			if ((cmd[i] == ';' || cmd[i] == '|') && !f_dquote && !f_squote)
			{
				data->flags->pipe = true;
				i += 2;
				return (i);
			}
			if (cmd[i] == '\"' && !f_squote)
				f_dquote = !f_dquote;
			if (cmd[i] == '\'' && !f_dquote)
				f_squote = !f_squote;
			if (cmd[i] == ' ' && cmd[i + 1] && cmd[i + 1] != ' ' && !f_dquote && !f_squote)
			{
				parse_string(data, cmd, array_index, i, j);
				array_index++;
				j = i + 1;
			}
			i++;
		}
		if (cmd[i] || !data->flags->heredoc)
		{
			parse_string(data, cmd, array_index, i, j);
			data->argv[++array_index] = NULL;
		}
		else
			data->argv[array_index] = NULL;
		return (i);
	}
	data->flags->error = true;
	return (0);
}