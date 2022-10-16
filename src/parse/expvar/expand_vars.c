/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 09:46:57 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/16 06:25:37 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	check_var_exists(t_data *data, char *var)
{
	int		i;
	char	*tmp;

	i = 0;
	if ((var && (var[0] == '~' || !ft_strncmp(var, "$?", 2)))
		|| (var && var[0] && var[1] && isnumeric(var[1])))
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

static void	expand_vars_join(t_data *data)
{
	data->expvar.str_before_vplus_vcontent
		= ft_strjoin(data->expvar.str_before_v, data->expvar.vcontent);
	data->expvar.str_after_v
		= ft_strdup(data->argv[data->expvar.i_arg]
			+ data->expvar.i_char + ft_strlen(data->expvar.vname));
	free (data->argv[data->expvar.i_arg]);
	if (ft_strlen(data->expvar.str_before_vplus_vcontent) > 0
		|| ft_strlen(data->expvar.str_after_v) > 0)
		data->argv[data->expvar.i_arg]
			= ft_strjoin(data->expvar.str_before_vplus_vcontent,
				data->expvar.str_after_v);
	else
		data->argv[data->expvar.i_arg] = ft_strdup("");
	free_str(3,
		data->expvar.str_before_vplus_vcontent,
		data->expvar.str_after_v, data->expvar.vcontent);
}

static bool	do_expand(t_data *data)
{
	data->expvar.str_before_v
		= ft_substr(data->argv[data->expvar.i_arg], 0, data->expvar.i_char);
	data->expvar.vname
		= ft_substr(data->argv[data->expvar.i_arg], data->expvar.i_char,
			strlen_path(data->argv[data->expvar.i_arg] + data->expvar.i_char));
	if (v_ex(data))
		return (true);
	if (data->argv[data->expvar.i_arg][data->expvar.i_char]
		&& data->argv[data->expvar.i_arg][data->expvar.i_char + 1]
		&& !ft_strncmp(data->argv[data->expvar.i_arg]
		+ data->expvar.i_char, "$?", 2))
		expand_vars_exit(data);
	else if (data->argv[data->expvar.i_arg][data->expvar.i_char + 1]
		&& isnumeric(data->argv[data->expvar.i_arg][data->expvar.i_char + 1]))
		expand_vars_shl(data);
	else
		data->expvar.vcontent = get_var_content(data, data->expvar.vname);
	if (data->expvar.vcontent)
		expand_vars_join(data);
	free_str(2, data->expvar.str_before_v, data->expvar.vname);
	if (data->expvar.i_char > ft_strlen(data->argv[data->expvar.i_arg]))
		return (false);
	return (true);
}

static bool	expand_vars_handle_arg(t_data *data)
{
	if (data->argv[data->expvar.i_arg][data->expvar.i_char] == '\"')
		data->expvar.f_dquote = !data->expvar.f_dquote;
	if (data->argv[data->expvar.i_arg][data->expvar.i_char] == '\''
		&& !data->expvar.f_dquote)
		data->expvar.f_squote = !data->expvar.f_squote;
	if (data->argv[data->expvar.i_arg][data->expvar.i_char] == '\\')
	{
		data->expvar.i_char += 2;
		return (true);
	}
	if ((!data->flags->noenv
			&& data->argv[data->expvar.i_arg][0] == '~'
		&& (!ft_isalnum(data->argv[data->expvar.i_arg][1])
		&& !data->expvar.f_dquote && !data->expvar.f_squote))
		|| (data->argv[data->expvar.i_arg][data->expvar.i_char] == '$'
		&& (data->argv[data->expvar.i_arg][data->expvar.i_char + 1])
		&& (data->argv[data->expvar.i_arg][data->expvar.i_char + 1] != ' ')
	&& (!data->expvar.f_squote)))
		return (do_expand(data));
	if (data->argv[data->expvar.i_arg][data->expvar.i_char]
		&& data->argv[data->expvar.i_arg][data->expvar.i_char + 1])
		return (data->expvar.i_char++, true);
	return (false);
}

bool	expand_vars(t_data *data)
{
	expand_vars_init(data, true);
	while (data->argv[data->expvar.i_arg])
	{
		expand_vars_reset_flags(data);
		while (data->argv[data->expvar.i_arg][data->expvar.i_char])
		{
			if (expand_vars_handle_arg(data))
				continue ;
			break ;
		}
		if (!data->argv[data->expvar.i_arg])
			break ;
		if (ft_strlen(data->argv[data->expvar.i_arg]) > 2)
			expand_vars_rm_mod(data);
		else if (ft_strncmp(data->argv[0], "echo", 4)
			&& (!ft_strncmp(data->argv[data->expvar.i_arg], "\'\'", 2)
				|| !ft_strncmp(data->argv[data->expvar.i_arg], "\"\"", 2)))
			expand_vars_weird_special_case(data, true);
		if (!data->argv[data->expvar.i_arg][0]
			&& data->argc == data->expvar.i_arg)
			expand_vars_weird_special_case(data, false);
		expand_vars_init(data, false);
	}
	return (true);
}
