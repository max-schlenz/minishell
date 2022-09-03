/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:25:20 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/03 13:17:50 by mschlenz         ###   ########.fr       */
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