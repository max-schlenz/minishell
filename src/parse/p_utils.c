/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:12:51 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/04 13:17:35 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*check_esc_var_quo(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
			return ((char *)s + i);
		i++;
	}
	if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
		return ((char *)s + i);
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
