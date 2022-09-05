/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 12:10:03 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/05 15:17:51 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	alloc_mem_array(t_data *data, char *cmd)
{
	int		wc;
	int		i;
	bool	f_dquote;
	bool	f_squote;

	wc = 0;
	i = 0;
	f_dquote = false;
	f_squote = false;
	while (cmd[i] && cmd[i] != '|')
	{
		if (cmd[i] == '\"' && !f_squote)
			f_dquote = !f_dquote;
		if (cmd[i] == '\'' && !f_dquote)
			f_squote = !f_squote;
		if (cmd[i] == ' ' && !f_dquote && !f_squote)
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

static char	*parse_string(t_data *data, char *cmd, int array_index, int i, int j)
{
	char *tmp_str;
	
	tmp_str = ft_substr(cmd, j, i - j);
	// if (tmp_str[0] == '\'' || tmp_str[0] == '\"')
	// 	tmp_str = ft_substr(tmp_str, 1, ft_strlen(tmp_str) - 2);
	data->argv[array_index] = ft_strdup(tmp_str);
	free(tmp_str);
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
		{
			return (ft_substr(data->envp[i], len_var + 1, ft_strlen(data->envp[i])));
		}
		i++;
	}
	return (ft_strdup(""));
}

void	expand_vars(t_data *data)
{
	int i_arg;
	int	i_char;

	char *str_before_v;
	char *vname;
	char *vcontent;
	char *str_before_vplus_vcontent;
	int	len_s_b_vp_vc = 0;

	i_arg = 0;
	i_char = 0;
	while (data->argv[i_arg])
	{
		while (data->argv[i_arg][i_char])
		{
			if (data->argv[i_arg][i_char] == '$')
			{
				str_before_v = ft_substr(data->argv[i_arg], 0, i_char);
				vname = ft_substr(data->argv[i_arg], i_char, strlen_path(data->argv[i_arg]));
				vcontent = get_var_content(data, vname);
				str_before_vplus_vcontent = ft_strjoin(str_before_v, vcontent);
				free (data->argv[i_arg]);
				data->argv[i_arg] = str_before_vplus_vcontent;
				// len_s_b_vp_vc = ft_strlen(str_before_vplus_vcontent);
				free (vname);
				free (vcontent);
			}
			i_char++;
		}
		// if (!len_s_b_vp_vc)
		// {
			
		// }
		printf("%s\n", data->argv[i_arg]);
		i_char = 0;
		i_arg++;
	}
}

bool	split_quotes(t_data *data, char *cmd)
{
	int		i;
	int		j;
	int		array_index;
	char	*tmp_str;
	bool	f_dquote;
	bool	f_squote;

	f_dquote = false;
	f_squote = false;
	if (alloc_mem_array(data, cmd))
	{
		i = 0;
		j = 0;
		array_index = 0;
		while (cmd[i] && cmd[i] != '|')
		{
			if (cmd[i] == '\"' && !f_squote)
				f_dquote = !f_dquote;
			if (cmd[i] == '\'' && !f_dquote)
				f_squote = !f_squote;
			if (cmd[i] == ' ' && !f_dquote && !f_squote)
			{
				parse_string(data, cmd, array_index, i, j);
				array_index++;
				j = i + 1;
			}
			i++;
		}
		parse_string(data, cmd, array_index, i, j);
		array_index++;
		data->argv[array_index] = NULL;
		expand_vars(data);
		return (true);
	}
	return (false);
}
