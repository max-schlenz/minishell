/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 13:31:42 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/04 13:33:31 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

void	make_btree(t_data *data)
{
	t_btree *tmp;
	
	parse_cmd(data);
	tmp = (*data->btree);
	while (tmp)
	{
		data->counter_btree++;
		tmp = tmp->left;
	}
	data->counter_btree--;
	// visualize(*data->btree);
}


void	set_btree_value(char *s, char *set, t_btree **head)
{
	int	i;
	char	*left;
	char	*right;
	t_btree *node_right;
	t_btree *new_node;
	t_btree *node_left;
	int		index;
	int		len;
	int		len_delim;


	i = 0;
	if (!set)
	{
		*head = lstnew_btree(" ");
		node_left = lstnew_btree(s);	
		lstadd_left(head, node_left);
		return ;
	}

	len = ft_strlen(s);
	len_delim = ft_strlen(set);
	while (len--)
	{
		if (ft_strncmp(s + len, set, len_delim) == 0)
		{
			
			if (!*head)
				*head = lstnew_btree(set);
			(*head)->value = set;
			node_right = lstnew_btree(ft_substr(s, len + len_delim, ft_strlen(s) - len - len_delim));
			lstadd_right(head, node_right);
			break;
		}
	}
	node_left = lstnew_btree(ft_substr(s, 0, len));
	lstadd_left(head, node_left);
}

char	*get_next_special_char(char *str)
{
	char	*tmp;
	int		i;

	tmp = NULL;
	i = 0;
	while (*str)
	{
		if (ft_strncmp(str, ">>", 2) == 0)
			tmp = ft_strdup(">>");
		else if (ft_strncmp(str, "<<", 2) == 0)
			tmp = ft_strdup("<<");
		else if (ft_strncmp(str, "||", 2) == 0)
			tmp = ft_strdup("||");
		else if (ft_strncmp(str, "&&", 2) == 0)
			tmp = ft_strdup("&&");
		else if (*str == '&' && (!i || *(str - 1) != '&'))
			tmp = ft_strdup("&");
		else if (*str == '|' && (!i || *(str - 1) != '|'))
			tmp = ft_strdup("|");
		else if (*str == '>' && (!i || *(str - 1) != '>'))
			tmp = ft_strdup(">");
		else if (*str == '<' && (!i || *(str - 1) != '<'))
			tmp = ft_strdup("<");
		str++;
		i++;
	}
	return (tmp);
}


static void exec_cmd2(t_data *data, char **envp)
{
	if (!builtins(data))
		exec_program(data);
}

static void exec_cmd(t_data *data, char **envp)
{
	if (!(*data->btree)->left && !(*data->btree)->right)
		return ;
	(*data->btree) = (*data->btree)->left;
	exec_cmd2(data, envp);
	if (!ft_strncmp((*data->btree)->value, "|", 2))
	{
		data->flag_pipe = true;
		// return ;	
	}
	parse_args(data, (*data->btree)->value);
}

//
while (data->counter_btree > 0)
			{
				if ((*data->btree)->left)
				{
					parse_args(data, (*data->btree)->left->value);
					if (!ft_strncmp((*data->btree)->value, "|", 2))
						data->flag_pipe = true;
					if (!builtins(data))
						exec_program(data);
				}
				if ((*data->btree)->right)
				{
					parse_args(data, ft_strtrim((*data->btree)->right->value, " "));
					if (!builtins(data))
						exec_program(data);
				}
				data->counter_btree--;
				if ((*data->btree)->left)
					(*data->btree) = (*data->btree)->left;
			}