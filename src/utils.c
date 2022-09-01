/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/01 16:41:59 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

void	set_btree_value(char *s, char *set, t_btree **btree)
{
	int	i;
	char	*left;
	char	*right;
	t_btree *node_right;
	t_btree *node_left;
	int		index;
	int		len;


	i = 0;
	len = ft_strlen(s);
	while (len--)
	{
		if (ft_strncmp(s + len, set, 2) == 0)
		{
			if (!*btree)
				(*btree) = ft_mslstnew2(set);
			node_right = ft_mslstnew2(ft_substr(s, len + 2, ft_strlen(s - len)));
			ft_mslstadd_right(btree, node_right);
			break;
		}
	}
	node_left = ft_mslstnew2(ft_substr(s, 0, len));
	ft_mslstadd_left(btree, node_left);
	//printf("STR:    %s\nVAL:    %s\nLEFT:   %s\nRIGHT: %s\n",s , set, (*btree)->left->value, (*btree)->right->value);
}

void	prioritization(t_data *data)
{
	// char 	*blub = ft_strdup("ls && grep && top && bla");
	t_btree	*btree = NULL;

	set_btree_value("ls | grep || bla || lol || more", "||", &btree);
	printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", btree->value, btree->left->value, btree->right->value);
	while (btree->left && ft_strstr2(btree->left->value, "||"))
	{
		set_btree_value(btree->left->value, "||", &btree);
		printf("VAL:    %s\nLEFT:   %s\nRIGHT: %s\n", btree->value, btree->left->value, btree->right->value);
		btree = btree->left;
	}
	// while (ft_strstr2(btree->right->value, "&&"))
	// 	ft_strstr(btree->right->value, "&&", btree);
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
	if (data)
		free(data);
	ft_exit(0);
}