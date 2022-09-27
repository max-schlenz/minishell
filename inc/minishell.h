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
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# define E_MEM_MSG	"Failed to allocate memory."
# define E_FORK_MSG	"Failed to create Forks."
# define E_NC_QUOTE "Error: unclosed quotes!"

# define E_TM_ARG "Error: too many arguments\n"
# define E_EXIT_REQNO "Error: exit: numeric argument required: "

# define WHITE "\x01\033[0m\x02"
# define RED "\x01\033[31;1m\x02"
# define GREEN "\x01\033[32;1m\x02"
# define YELLOW "\x01\033[33;1m\x02"
# define BLUE "\x01\033[34;1m\x02"
# define MAGENTA "\x01\033[35;1m\x02"
# define CYAN "\x01\033[36;1m\x02"

# define CFG ".mscfg"

typedef struct s_parser
{
	int		array_index;
	int		arg_start;
	bool	f_dquote;
	bool	f_squote;
}	t_parser;

typedef struct s_rmq
{
	size_t	start;
	size_t	end;
}	t_rmq;

typedef struct s_color
{
	char	*prompt[4];
	char	*prompt_tmp;
	char	*cfg_str_pf;
	char	*cfg_str_cl;
	char	*cfg_str;
	char	*prompt_tmp2;
	char	*code[2];
}	t_color;

typedef struct s_pipes
{
	int		pipefd[FD_SETSIZE][2];
}	t_pipes;

typedef struct s_flags {
	bool	error;
	bool	redir_out;
	bool	redir_in;
	bool	heredoc;
	bool	redir_append;
	bool	heredoc_begin;
	bool	and;
	bool	or;
	bool	pipe;
	bool	debug;
	bool	bracket;
	bool	prio;
	bool	noenv;
	bool	exit_code_of;
	bool	macos;
}	t_flags;

typedef struct s_data
{
	char		**path;
	char		**args;
	char		**envp;
	char		**argv;
	int			argc;
	char		**cmd_split;
	char		*cmd;
	int			counter_env;
	int			counter_pipes;
	int			fd_i;
	char		*prompt;
	char		*prompt_cl1;
	char		*prompt_cl2;
	char		*file_name;
	char		*file_name2;
	char		*heredoc_delim;
	char		*last_cmd;
	char		**wc_dir_content;
	int			exit_code;
	long long	exit_status;
	int			fd;
	int			mscfg;
	FILE		*debug;
	t_parser	parser;
	t_color		color;
	t_rmq		rmq;
	t_flags 	*flags;
	t_pipes		*pipes;
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
void			open_pipes(t_data *data);

//parse/envp.c
void			parse_envp(t_data *data, char **envp);
void			parse_path(t_data *data);
void			sort_array(t_data *data);
int				strcmp_alnum(const char *s1, const char *s2);

//utils/cleanup.c
void			cleanup(t_data *data, int flag);
void			ms_exit(t_status flag, int exit_status);
void			free_array(char **array);
void			close_pipes(t_data *data);

//utils/utils.c
void			history(t_data *data);

// this shouldn't even be in here but has to be for some reason
void			rl_replace_line(const char *text, int clear_undo);

bool			exec_program(t_data *data);

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
void			realloc_envp(t_data *data, char *newv, int flag);
size_t			strlen_var(const char *c);
int				strdiff(const char *s1, const char *s2);

//parse/split.c
bool			split_quotes(t_data *data, char *cmd, int *i);
bool			expand_vars(t_data *data);

//parse/syntax.c
bool			check_syntax(t_data *data, char *cmd);
bool			syntax_err(t_data *data, char *cmd);
bool			check_syntax_first_char(t_data *data, char *cmd);

//exec/builtins.c
bool			builtin_environment(t_data *data);
bool			builtin_print(t_data *data);
bool			builtin_cd(t_data *data);
bool			builtin_echo(t_data *data);
bool			builtin_export(t_data *data, char *setv);
bool			builtin_env(t_data *data);
bool			builtin_pwd(t_data *data);
bool			builtin_unset(t_data *data);
bool			builtin_color(t_data *data, char *cfg);
bool			builtin_history(t_data *data);

//exec/exec.c
void			pipes(t_data *data);

//utils/signal.c
void			signal_handler(int signal, siginfo_t *info, void *context);
void			signals(bool inter_active);

//utils/config.c
void			create_cfg();
void			read_cfg(t_data *data);

//parse/wildcard.c
char			*find_wc(t_data *data, char *cmd);

//parse/pre parse

char			*pre_parse(t_data *data, char *cmd);
char			*skip_d(t_data *data, char *cmd, char delim);

bool 			count_pipes(t_data *data, char *cmd);
char 			*strrepc(char *cmd, char to_rep, char rep_with);
char			*get_var_content(t_data *data, char *var);
void			free_argv(t_data *data, char **array);
int				isnumeric(int c);
size_t			strlen_expv(const char *c);

int				*get_indexes(char *str);

void			get_all_names(t_data *data);
void			redirs_pipes(t_data *data);
void			reset_pipes_flags(t_data *data);
int				isidentifier(int c);
void			dbg(t_data *data);
void			heredoc(t_data *data);
long long		ms_atoll(t_data *data, const char *str);
char			*strjoin_nl(char const *s1, char const *s2);
void			wait_for_childs(t_data *data);

#endif
