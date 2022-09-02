/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/02 17:48:36 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	strlen_path(const char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '/')
		i++;
	return (i);
}

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
	if (s[i] == '\'' || s[i] == '$' || s[i] == '\"')
		return ((char *)s + i);
	return (0);
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


	i = 0;
	len = ft_strlen(s);
	while (len--)
	{
		//printf("1 in here len %d\n", len);
		if (ft_strncmp(s + len, set, 2) == 0)
		{
			if (!*head)
				*head = ft_mslstnew2(set);
			(*head)->value = set;
			node_right = ft_mslstnew2(ft_substr(s, len + 2, ft_strlen(s) - len - 2));
			//printf("%c %c %zu\n", *(s + len), *(s + len + 3), ft_strlen(s) - len - 2);
			ft_mslstadd_right(head, node_right);
			break;
		}
	}
	//printf("in here len %d\n", len);
	node_left = ft_mslstnew2(ft_substr(s, 0, len));
	ft_mslstadd_left(head, node_left);
	//printf("wtf %d\n", len);
	//printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n",(*btree)->value, (*btree)->left->value, (*btree)->right->value);
}

void	prioritization(t_data *data, t_btree **btree)
{
	// char 	*blub = ft_strdup("ls && grep && top && bla");
	t_btree	*tmp;// = ft_calloc(1, sizeof(t_btree));

	
	set_btree_value("1 || 2 || 3 || 4 || 5 || 6 || 7", "||", btree);
	tmp = *btree;
	printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", tmp->value, tmp->left->value, tmp->right->value);
	tmp = tmp->left;
	//tmp = tmp->left;
	while (tmp && ft_strstr2(tmp->value, "||"))
	{
		//printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", tmp->value, tmp->left->value, tmp->right->value);
		//printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", (*btree)->value, (*btree)->left->value, (*btree)->right->value);
		
		set_btree_value(tmp->value, "||", &tmp);
		printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", tmp->value, tmp->left->value, tmp->right->value);
		tmp = tmp->left;
		
		
	}
	/*while ((*btree))
	{
		printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", (*btree)->value, (*btree)->left->value, (*btree)->right->value);
		(*btree) = (*btree)->left;
	}*/
}

void	lol(t_data *data)
{
	t_btree	**btree = ft_calloc(1, sizeof(t_btree));
	prioritization(data, btree);
	printf("------------------\n");
	while (*btree && (*btree)->right)
	{
		printf("VAL:    %s RIGHT: %s\n", (*btree)->value, (*btree)->right->value);
		(*btree) = (*btree)->left;
	}
}

void	ft_exit(t_status flag)
{
	// system("leaks minishell");
	if (flag == SUCCESS)
		exit(EXIT_SUCCESS);
	else if (flag == E_MEM)
		printf("%s", E_MEM_MSG);
	else if (flag == E_FORK)
		printf("%s", E_FORK_MSG);
	exit (EXIT_FAILURE);
}

void	cleanup(t_data *data, int flag)
{
	// if (data->env)
	// 	ft_lstclear(data);
	// if (data->args)
	// 	free(data->args);
	// if (data->path)
	// 	free (data->path);
	// if (data->cmd)
	// 	free(data->cmd);
	// ft_mslstclear(data);
	if (data)
		free(data);
	ft_exit(0);
	return ;
}