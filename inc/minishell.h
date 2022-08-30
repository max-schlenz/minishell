/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:55:52 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/30 15:28:09 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/uio.h>
# include <stdio.h>
# include <stddef.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define E_MEM	"Failed to allocate memory."
# define E_FORK	"Failed to create Forks."


typedef struct s_msdata
{
	char	**envp;
	char	*path;
	char	*cmd;
	char	**args;
	int		c_env_vars;
	//t_msenv	*env;
}					t_msdata;

//	init.c
t_msdata		*allocate_mem(void);

//	utils.c
void			ft_exit(int flag);
void			cleanup(t_msdata *data, int flag);

//	utils_lst.c
// void			ft_mslstdelone(t_msenv *lst);
// void			ft_mslstclear(t_msdata *data);
// void			ft_mslstadd_back(t_msenv **lst, t_msenv *new);
// void			ft_mslstadd_front(t_msenv **lst, t_msenv *new);
// t_msenv		*ft_mslstlast(t_msenv *lst);

//	utils_lst2.c
// size_t			ft_mslstsize(t_msenv *lst);
// t_msenv			*ft_mslstnew(t_msdata *data, char *var, char *content);

// this shouldn't even be in here but has to be for some reason
void			rl_replace_line(const char *text, int clear_undo);

#endif