

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
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define E_MEM_MSG	"Failed to allocate memory."
# define E_FORK_MSG	"Failed to create Forks."

typedef struct s_env
{
	char			*var;
	char			*content;
	struct s_env	*next;
	struct s_env	*last;
}	t_env;


typedef struct s_btree
{
	char			*value;
	struct s_btree	*left;
	struct s_btree	*right;
	struct s_btree	*next;
}	t_btree;

typedef struct s_pipes
{
	int		pipefd[2];
	char	*rdbuf;
	char	*out1;
	pid_t	pid[2];
}					t_pipes;

typedef struct s_data
{
	char	**path;
	char	**args;
	char	*cmd;
	int		counter_envv;
	t_env	*env;
	t_btree **btree;
	int		counter_btree;
	bool	flag_pipe;
	t_pipes	pipes;
	int		r_pipe;
}	t_data;


typedef enum s_status
{
	SUCCESS,
	E_MEM,
	E_FORK,
}	t_status;

//	init.c
void			init(t_data *data);
t_data			*allocate_mem(void);

//	parse_envp.c
void			parse_envp(t_data *data, char **envp);
void			parse_path(t_data *data);

//	utils.c
void			cleanup(t_data *data, int flag);
void			ft_exit(t_status flag);

//	utils_lst.c
void			ft_mslstdelone(t_env *lst);
void			ft_mslstclear(t_data *data);
t_env			*ft_mslstlast(t_env *lst);
void			ft_mslstadd_back(t_env **lst, t_env *new);
void			ft_mslstadd_front(t_env **lst, t_env *new);
t_btree			*ft_mslstnew2(char *content);
void			ft_mslstadd_back2(t_btree **lst, t_btree *new);

//	utils_lst2.c
size_t			ft_mslstsize(t_env *lst);
t_env			*ft_mslstnew(t_data *data, char *var, char *content);

// this shouldn't even be in here but has to be for some reason
void			rl_replace_line(const char *text, int clear_undo);

void	ft_mslstadd_left(t_btree **lst, t_btree *new);
void	ft_mslstadd_right(t_btree **lst, t_btree *new);
t_btree	*ft_mslstnew2(char *content);
void	exec_program(t_data *data, char **envp);
char	*chk_escvar(const char *s);
size_t	strlen_path(const char *c);
bool	builtins(t_data *data);
void	parse_args(t_data *data);

//parsing
void	prioritization(t_data *data);
void	lol(t_data *data);

//visualize btree
void	visualize(t_btree *head);


#endif