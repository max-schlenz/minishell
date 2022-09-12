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
# include <readline/readline.h>
# include <readline/history.h>

# define E_MEM_MSG	"Failed to allocate memory."
# define E_FORK_MSG	"Failed to create Forks."
# define E_NC_QUOTE "Error: unclosed quotes!"

# define WHITE "\033[0m"
# define RED "\033[31;1m"
# define GREEN "\033[32;1m"
# define YELLOW "\033[33;1m"
# define BLUE "\033[34;1m"
# define MAGENTA "\033[35;1m"
# define CYAN "\033[36;1m"

# define CFG ".mscfg"

typedef struct s_pipes
{
	int		pipefd[4096][2];
}	t_pipes;

typedef struct s_flags {
	bool	error;
	bool	redir_out;
	bool	redir_in;
	bool	redir_in_delim;
	bool	redir_out_append;
	bool	and;
	bool	or;
	int		pipe;
}	t_flags;

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
	int		fd_i;
	char	*prompt;
	char	*prompt_cl1;
	char	*prompt_cl2;
	char	*file_name;
	char	*heredoc_delim;
	char	*last_cmd;
	char	**wc_dir_content;
	int		exit_status;
	bool	exit;
	int		fd;
	int		mscfg;
	t_flags *flags;
	t_pipes *pipes;
}	t_data;

typedef enum s_status
{
	SUCCESS,
	E_MEM,
	E_FORK,
}	t_status;

//	init.c
void			init_vars(t_data *data, char **argv);
t_data			*allocate_mem();
void			open_pipes(t_data *data);

//parse/envp.c
void			parse_envp(t_data *data, char **envp);
void			parse_path(t_data *data);
void			sort_array(t_data *data);
int				strcmp_alnum(const char *s1, const char *s2);


//utils/cleanup.c
void			cleanup(t_data *data, int flag);
void			ft_exit(t_status flag);
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
void			realloc_envp(t_data *data, int flag);
size_t			strlen_var(const char *c);
int				strdiff(const char *s1, const char *s2);

//parse/split.c
char			*split_quotes(t_data *data, char *cmd);
char			*insert_space(char *cmd, int index);
char			*pre_parse(char *cmd);
void			expand_vars(t_data *data);

//parse/syntax.c
bool			check_syntax(t_data *data);

//exec/builtins.c
bool			builtin_environment(t_data *data);
bool			builtin_print(t_data *data);
bool			builtin_cd(t_data *data);
bool			builtin_echo(t_data *data);
bool			builtin_export(t_data *data);
bool			builtin_env(t_data *data);
bool			builtin_pwd(t_data *data);
bool			builtin_unset(t_data *data);
bool			builtin_color(t_data *data, char *cfg);
bool			builtin_history(t_data *data);

//exec/exec.c
void			pipes(t_data *data);

//utils/signal.c
void			signal_handler(int sig, siginfo_t *info, void *context);
void			signals();

//utils/config.c
void			create_config(t_data *data);
void			read_config(t_data *data);

//parse/wildcard.c
char			*find_wc(t_data *data, char *cmd);

//parse/pre parse

char			*insert_space(char *cmd, int index);
char			*pre_parse(char *cmd);
void			skip_spaces(char *cmd, int *i);

#endif
