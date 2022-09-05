/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_cmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 14:41:09 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/05 15:56:34 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void parse_var(t_data *data, char *arg, int index_arg)
{
	char	*rest;
	int		len_var;
	int		i;

	arg++;
	len_var = strlen_path(arg);
	i = 0;
	while (data->envp[i])
	{
		if (!ft_strncmp(arg, data->envp[i], len_var))
			break ;
		i++;
	}
	if (data->envp[i])
		data->argv[index_arg] = ft_strdup(data->envp[i] + len_var + 1);
	else
		data->argv[index_arg] = "";
	rest = ft_substr(arg, len_var, ft_strlen(arg) - len_var);
	data->argv[index_arg] = ft_strjoin(data->argv[index_arg], rest);
}

void	parse_args(t_data *data, int cmd_index)
{
	int		index_args;
	char	*var;

	index_args = 0;
	// data->args = ft_split(data->cmd_split[cmd_index], ' ');
	while (data->argv[index_args])
	{
		if (check_esc_var_quo(data->argv[index_args]))
		{
			var = check_esc_var_quo(data->argv[index_args]);
			if (var[0] == '$')
				parse_var(data, var, index_args);
			else if (var[0] == '\'')
				data->argv[index_args] = ft_substr(var, 1, ft_strlen(var) - 1);
			else if (var[0] == '\"')
			{
				var = ft_substr(var, 1, ft_strlen(var) - 2);
				if (var[0] == '$')
					parse_var(data, var, index_args);
			}
		}
		printf("%s\n", data->argv[index_args]);
		data->argv[index_args] = ft_strtrim(data->argv[index_args], "\"");
		data->argv[index_args] = ft_strtrim(data->argv[index_args], "\'");
		if (index_args > 0)
			data->argv[index_args] = ft_strjoin(data->argv[index_args], " ");
		index_args++;
	}
	data->argv[index_args] = NULL;
}
