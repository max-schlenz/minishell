/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_cmd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 14:41:09 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 18:49:21 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void parse_var(t_data *data, char *var, int i)
{
	char	*rest;
	int		j;
	int		len_var;
	
	int g = 0;

	var++;
	len_var = strlen_path(var);
	while (j < data->counter_envv)
	{
		if (!ft_strncmp(var, data->env->var, len_var))
			break ;
		data->env = data->env->next;
		j++;
	}
	if (j >= data->counter_envv)
		data->args[i] = ft_strdup("");
	else
		data->args[i] = ft_strdup(data->env->content);
	rest = ft_substr(var, len_var, ft_strlen(var) - len_var);
	data->args[i] = ft_strjoin(data->args[i], rest);
}

void	parse_args(t_data *data, char *cmd)
{
	int		i;
	char	*var;
	char	*tmp;

	// printf("%d\n", data->counter_btree);
	// data->args = ft_split((*data->btree)->left->value, ' ');
	i = 0;
	data->args = ft_split(cmd, ' ');
	while (data->args[i])
	{
		if (check_esc_var_quo(data->args[i]))
		{
			var = check_esc_var_quo(data->args[i]);
			if (var[0] == '$')
				parse_var(data, var, i);
			else if (var[0] == '\'')
				data->args[i] = ft_substr(var, 1, ft_strlen(var) - 1);
			else if (var[0] == '\"')
			{
				var = ft_substr(var, 1, ft_strlen(var) - 2);
				if (var[0] == '$')
					parse_var(data, var, i);
			}
		}
		data->args[i] = ft_strtrim(data->args[i], "\"");
		data->args[i] = ft_strtrim(data->args[i], "\'");
		data->args[i] = ft_strjoin(data->args[i], " ");
		i++;
	}
	i = 0;
	while (data->args[i])
	{
		printf("%s\n", data->args[i++]);
	}
}

void	parse_cmd(t_data *data)
{
	t_btree	*tmp;
	char	*next_delim;

	next_delim = get_next_special_char(data->cmd);
	if (!(*data->btree))
	{
		set_btree_value(data->cmd, next_delim, data->btree);
		tmp = (*data->btree);
		tmp = tmp->left;	
	}
	while (tmp && next_delim)
	{
		next_delim = get_next_special_char(tmp->value);
		set_btree_value(tmp->value, next_delim, &tmp);
		tmp = tmp->left;
	}
}

void	lol(t_data *data)
{
	t_btree *tmp;
	
	prioritization(data);
	tmp = (*data->btree);
	while (tmp)
	{
		data->counter_btree++;
		tmp = tmp->left;
	}
	data->counter_btree--;
	// visualize(*data->btree);
}