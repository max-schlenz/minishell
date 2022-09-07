/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 18:55:52 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/06 11:50:44 by mschlenz         ###   ########.fr       */
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
# define E_NC_QUOTE "Error: unclosed quotes!"

typedef struct s_data
{
	char	**path;
	char	**args;
	char	**envp;
	char	**argv;
	char	**cmd_split;
	char	*cmd;
	int		counter_env;
	int		counter_pipes;
	int		flag_pipe;
	bool	flag_error;
	int		pipefd[4096][2];
	int		fd_i;
}	t_data;

typedef enum s_status
{
	SUCCESS,
	E_MEM,
	E_FORK,
}	t_status;

//	init.c
void			init_vars(t_data *data);
t_data			*allocate_mem();

//parse/envp.c
void			parse_envp(t_data *data, char **envp);
void			parse_path(t_data *data);
void			sort_array(t_data *data);


//utils/exit.c
void			cleanup(t_data *data, int flag);
void			ft_exit(t_status flag);
void			free_array(char **array);


// this shouldn't even be in here but has to be for some reason
void			rl_replace_line(const char *text, int clear_undo);

void			exec_program(t_data *data);

size_t			strlen_path(const char *c);
bool			builtins(t_data *data);

//parsing
void			prioritization(t_data *data);
void			make_btree(t_data *data);
char			*check_esc_var_quo(const char *s);
void			parse_args(t_data *data, int cmd_index);

//parse/utils.c
char			*get_next_special_char(char *str);
size_t			strlen_path(const char *c);
char			*check_esc_var_quo(const char *s);
void			realloc_envp(t_data *data, int flag);
size_t			strlen_var(const char *c);

//parse/split_quotes.c
char			*split_quotes(t_data *data, char *cmd);

//exec/builtins.c
bool			builtin_cd(t_data *data);
bool			builtin_echo(t_data *data);
bool			builtin_export(t_data *data);
bool			builtin_env(t_data *data);
bool			builtin_pwd(t_data *data);

//utils/signal.c
void			signal_handler(int sig, siginfo_t *info, void *context);

#endif