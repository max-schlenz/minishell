

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:55:52 by mschlenz          #+#    #+#             */
/*   Updated: 2022/08/30 14:14:30 by mschlenz         ###   ########.fr       */
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

typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
	struct s_env	*last;
	
}					t_env;

typedef struct s_data
{
	char	**envp;
	char	*path;
	char	*cmd;
	char	**args;
	t_env	*env;
	int		c_env_vars;
	char	*argv1;
	// int	file1;
	// int	file2;
	// char	**args2;
	// char	*path2;
}					t_data;

//	init.c
void			init(t_data *data);
t_data		*allocate_mem(void);

//	utils.c
void			cleanup(t_data *data, int flag);

//	utils_lst.c
void			ft_lstdelone(t_env *lst);
void			ft_lstclear(t_data *data);
t_env			*ft_lstlast(t_env *lst);
void			ft_lstadd_back(t_env **lst, t_env *new);
void			ft_lstadd_front(t_env **lst, t_env *new);

//	utils_lst2.c
size_t			ft_lstsize(t_env *lst);
t_env			*ft_lstnew(t_data *data, char *var, char *content);

// this shouldn't even be in here but has to be for some reason
void			rl_replace_line(const char *text, int clear_undo);

#endif