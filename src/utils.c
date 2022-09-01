/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/01 14:24:00 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_strstr(const char *s, char *set)
{
	int	i;
	char	*left;
	char	*right;
	t_btree *node1;
	t_btree *node2;
	t_btree *btree = NULL;// ft_calloc(1, sizeof(t_btree));


	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == set[0] && s[i + 1] && s[i + 1] == set[1])
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
	return (0);
}

void	prioritization(t_data *data)
{
		ft_strstr("ls | grep || bla", "||");
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