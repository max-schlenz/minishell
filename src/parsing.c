/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 14:59:35 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/02 19:43:16 by tdehne           ###   ########.fr       */
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

bool	ft_strstr2(const char *s, char *set)
{
	int	i;
	char	*left;
	char	*right;
	t_btree *node1;
	t_btree *node2;
	t_btree *btree = NULL;// ft_calloc(1, sizeof(t_btree));
	int		index;


	i = 0;
	while (s[i] != '\0')
	{
		if (ft_strncmp(s + i, set, 2) == 0)
			return (true);
		i++;
	}
	return (false);
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
	len = ft_strlen(s);
	len_delim = ft_strlen(set);
	while (len--)
	{
		//printf("\ns + len = %s set = %s len = %d\n", s+len, set, len_delim);
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

	tmp = NULL;
	while (*str)
	{
		if (ft_strncmp(str, ">>", 2) == 0)
			tmp = ft_strdup(">>");
		else if (ft_strncmp(str, "<<", 2) == 0)
			tmp = ft_strdup("<<");
		else if (*str == '|')
			tmp = ft_strdup("|");
		else if (*str == '>' && *(str - 1) != '>')
			tmp = ft_strdup(">");
		else if (*str == '<' && *(str - 1) != '<')
			tmp = ft_strdup("<");
		str++;
	}
	//printf("%s\n", tmp);
	return (tmp);
}

void	prioritization(char *data, char *delim, t_btree **btree)
{
	// char 	*blub = ft_strdup("ls && grep && top && bla");
	t_btree	*tmp;// = ft_calloc(1, sizeof(t_btree));
	char	*next_delim;

	next_delim = get_next_special_char(data);
	//printf("%s\n", next_delim);
	if (!*btree)
	{
		//printf("init, %s\n", next_delim);
		set_btree_value(data, next_delim , btree);
		tmp = *btree;
		printf("val %s\n", (*btree)->value);
		tmp = tmp->left;
		printf("val %s\n", (*btree)->left->value);
		printf("val %s\n", (*btree)->value);
		//free(next_delim);
		next_delim = get_next_special_char(tmp->value);
		
	}
	//printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", tmp->value, tmp->left->value, tmp->right->value);
	//ft_strstr2(tmp->value, delim)
	//printf("VAL:%s\n", tmp->value);
	while (tmp && next_delim)
	{
		printf("val before:%s delim %s\n", (*btree)->value, next_delim);
		set_btree_value(tmp->value, next_delim, &tmp);
		printf("val after:%s delim %s\n", (*btree)->value, next_delim);
		tmp = tmp->left;
		//free(next_delim);
		next_delim = get_next_special_char(tmp->value);
	}
}

/*int	index_last_delim(char *str, char *delim)
{
	int	delim_len;
	int	index;
	int	i;

	delim_len = ft_strlen(delim);
	index = 0;
	i = 0;
	while (str[i])
	{
		if (ft_strncmp(str + i, delim, delim) == 0)
			index = i;	
		i++;
	}
	return (index);
}



char	*cut_str(char *str, char *delim)
{
	char	*new_str;
	int		len;
	int		i;

	len = index_last_delim(str, delim) + 1;
	new_str = ft_calloc(len + 1, sizeof(char));
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	free(str);
	str = NULL;
	return (new_str);
}*/


void	lol(t_data *data)
{
	t_btree	**btree = ft_calloc(1, sizeof(t_btree));
	prioritization("lol | grep x >> cat lol", ">>" , btree);
	//prioritization("grep x >> cat lol", ">>" , btree);
	visualize(*btree);
	/*while (*btree && (*btree)->right && (*btree)->left)
	{
		printf("VAL:    %s RIGHT: %s LEFT: %s\n", (*btree)->value, (*btree)->right->value, (*btree)->left->value);
		(*btree) = (*btree)->left;
	}*/
}