/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 14:59:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 13:30:17 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*chk_escvar(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
			return ((char *)s + i);
		i++;
	}
	/*if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
		return ((char *)s + i);*/
	return (0);
}

size_t	strlen_path(const char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '/')
		i++;
	return (i);
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
		*head = ft_mslstnew2(s);
		//exit(0);
		return ;
	}

	len = ft_strlen(s);
	len_delim = ft_strlen(set);
	while (len--)
	{
		if (ft_strncmp(s + len, set, len_delim) == 0)
		{
			
			if (!*head)
				*head = ft_mslstnew2(set);
			(*head)->value = set;
			node_right = ft_mslstnew2(ft_substr(s, len + len_delim, ft_strlen(s) - len - len_delim));
			ft_mslstadd_right(head, node_right);
			break;
		}
	}
	node_left = ft_mslstnew2(ft_substr(s, 0, len));
	ft_mslstadd_left(head, node_left);
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

void	prioritization(char *data, t_btree **btree)
{
	t_btree	*tmp;
	char	*next_delim;

	next_delim = get_next_special_char(data);
	if (!*btree)
	{
		set_btree_value(data, next_delim , btree);
		tmp = *btree;
		tmp = tmp->left;
		//next_delim = get_next_special_char(tmp->value);
		
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
	t_btree	**btree = ft_calloc(1, sizeof(t_btree));
	prioritization(data->cmd, btree);
	visualize(*btree);
}