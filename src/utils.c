/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/01 15:07:21 by tdehne           ###   ########.fr       */
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

char	*ft_strstr(char *s, char *set)
{
	int	i;
	char	*left;
	char	*right;
	t_btree *node1;
	t_btree *node2;
	t_btree *btree = NULL;
	int		index;


	i = 0;
	while (s[i] != '\0')
	{
		if (ft_strncmp(s + i, set, 2) == 0)
		{
			if (!btree)
				btree = ft_mslstnew2(set);
			node1 = ft_mslstnew2(ft_substr(s, i + 2, ft_strlen(s - i)));
			ft_mslstadd_right(&btree, node1);
			break;
		}
		i++;
	}
	node2 = ft_mslstnew2(ft_substr(s, 0, i));
	ft_mslstadd_left(&btree, node2);
	printf("STR:    %s\nVAL:    %s\nLEFT:   %s\nRIGHT: %s\n",s , set, btree->left->value, node1->value);
	return (0);
}

void	prioritization(t_data *data)
{
	// char 	*blub = ft_strdup("ls && grep && top && bla");
	ft_strstr("ls | grep || bla", "||");
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