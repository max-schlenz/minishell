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

# include <strings.h>
# include <data.h>

# define CFG ".mscfg"
# define DBG ".debug"

//	init.c
void			init_vars(t_data *data);
t_data			*allocate_mem(void);
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

//parse/argv.c
bool			split_quotes(t_data *data, char *cmd, int *i);
bool			expand_vars(t_data *data);
void			split_reset_flags(t_data *data);

//parse/syntax.c
bool			check_syntax(t_data *data, char *cmd);
bool			syntax_err(t_data *data, char *cmd);
bool			check_syntax_first_char(t_data *data, char *cmd);

//exec/builtins.c
bool			builtin(t_data *data);
bool			builtin_cd(t_data *data);
bool			builtin_echo(t_data *data);
bool			builtin_export(t_data *data, char *setv);
bool			builtin_env(t_data *data);
bool			builtin_pwd(t_data *data);
bool			builtin_unset(t_data *data);
bool			builtin_color(t_data *data, char *cfg);

//exec/exec.c
void			pipes(t_data *data);

//utils/signal.c
void			signal_handler(int signal, siginfo_t *info, void *context);
void			signals(bool inter_active);

//utils/config.c
void			read_cfg(t_data *data);

//parse/wildcard.c
char			*find_wc(t_data *data, char *cmd);

//parse/pre parse

char			*pre_parse(t_data *data, char *cmd);
char			*skip_d(t_data *data, char *cmd, char delim);

bool			count_pipes(t_data *data, char *cmd);
char			*strrepc(char *cmd, char to_rep, char rep_with);
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
bool			builtin_dbg(t_data *data);
char			*get_path(t_data *data, char *cmd);
bool			random_cl(t_data *data);
bool			builtin_rcl(t_data *data);
char			*handle_heredoc(t_data *data, char *cmd);
void			heredoc_prompt(t_data *data);
void			wr_tmp_file(t_data *data);
void			wr_new_cmd(t_data *data, char **cmd, int *i);
void			rm_tmp_files(t_data *data);
void			init_hd(t_data *data);
void			free_hd(t_data *data);
char			*merge_str(int index, ...);
void			free_str(int index, ...);
char			*heredoc_delim(char *cmd, int i, int j);
void			prio(t_data *data, char *cmd, int *i);
void			builtin_fork(t_data *data, bool flag);

char			*str_realloc(char *ptr, char *new, bool free_new);
char			*strmv(char *new);

// utils/prompt.c

int				prompt(t_data *data, char *cmd, int flag);

// exec/builtins/cd/cd.c
bool			builtin_cd(t_data *data);
void			free_cd(t_data *data);

// exec/builtins/cd/cd_cleanup.c
bool			cd_success(t_data *data, int i);
void			init_cd(t_data *data, int *index_pwd);
void			free_cd(t_data *data);
bool			cd_err(t_data *data);

// exec/builtins/cd/cd_utils.c
size_t			cd_find_pwd(t_data *data);
void			cd_root(t_data *data);

// exec/builtins/echo/echo.c
bool			builtin_echo(t_data *data);
void			remove_quotes(t_data *data, int i_arg);
void			remove_backslashes(t_data *data, int i_arg);

// parse/expvar/utils.c
void			expand_vars_init(t_data *data, bool flag);
void			expand_vars_rm_mod(t_data *data);
void			expand_vars_reset_flags(t_data *data);
void			expand_vars_weird_special_case(t_data *data, bool flag);
bool			v_ex(t_data *data);

// parse/expvar/special_cases.c
void			expand_vars_ne(t_data *data);
void			expand_vars_exit(t_data *data);
void			expand_vars_shl(t_data *data);

//parse/expvar/expand_vars.c
bool			check_var_exists(t_data *data, char *var);

//parse/expvar/get_var.c
char			*get_var_content(t_data *data, char *var);

//parse/argv/modifiers.c
bool			parse_string(t_data *data, char *cmd, int i, bool end);
bool			parse_or(t_data *data, int *i, int start_args);
bool			parse_and(t_data *data, int *i, int start_args);
bool			parse_pipes(t_data *data, int *i);
bool			parse_redir_out(t_data *data, char *cmd, int *i);

//parse/argv/quote_escape.c
char			*rm_quotes_start(t_data *data, int *i, int i_arg, char *tmp);
void			rm_quotes_wr_argv(t_data *data, int i_arg, char *tmp);
char			*rm_quotes_mid(t_data *data, int *i, int i_arg, char *argv);
void			remove_quotes(t_data *data, int i_arg);
void			remove_backslashes(t_data *data, int i_arg);

//parse/argv/utils.c
bool			set_filenames(t_data *data, int *i, char *cmd, int flag);
void			rm_quotes_init(t_data *data);

//utils/history.c
bool			builtin_history(t_data *data);

//src/exec/builtins/export/error.c
bool			export_err_inv(t_data *data, char *setv);
bool			export_err_con(t_data *data, char *setv);
bool			export_err_op(t_data *data, char *setv);

//builtins/export/utils.c
bool			export_check_str(char *str);
bool			export_var(t_data *data, char *setv);
void			export_output(int len, char *name, char *val);

//builtins/export/export.c
void			export_setv(t_data *data, char *setv);

//builtins/exit/exit.c
bool			builtin_exit(t_data *data);

//debug.c 							!REMOVE!
void			dbg(t_data *data);

//exec/utils.c
bool			is_builtin(t_data *data);
bool			check_path(char *cmd);
void			exec_close_pipes(t_data *data);
void			exec_set_flags(t_data *data);

//parse/argv/argv_redir
int				split_redir(t_data *data, char *cmd, int *i);

//parse/argv/argv_ops
bool			split_andor(t_data *data, char *cmd, int *i, int start_args);
bool			split_esc(t_data *data, char *cmd, int *i);
bool			split_pipe(t_data *data, char *cmd, int *i);
bool			split_col(t_data *data, char *cmd, int *i);
bool			split_semicolon(t_data *data, char *cmd, int *i);

//parse/argv/argv_utils
void			split_qflags(t_data *data, char *cmd, int *i);
bool			alloc_mem_array(t_data *data, char *cmd);

//parse/pre_parse/pre_parse
char			*insert_space(t_data *data, char *cmd, int index);

//parse/pre_parse/utils
void			pre_parse_check_ops(t_data *data, char **cmd, char op, int i);

//parse/syntax/utils
bool			check_syntax_iter(char *cmd, int *i);
bool			*err_msg(char *err_c);
char			*err_type(t_data *data, char c, int exit_status, int flag);
bool			syntax_err_msg(t_data *data, char *ops, int i);
bool			check_syntax_helper(char *cmd, char *ops, int i);

//utils/color/utils
void			color_cleanup(t_data *data);
void			color_help(void);

//utils/prompt/utils
void			priorities(t_data *data, char **tmp_cmd, int *i);

//parse/wildcards/cases
bool			match_end(t_data *data, char *str, char *pattern);
bool			match_inbetween(t_data *data, char *str, char *pattern);
bool			match_start(t_data *data, char *str, char *pattern);

//parse/wildcards/utils
char			**realloc_argv(t_data *data, int argv_i, char *file, bool repl);

//parse/wildcards/match
char			**match_files(t_data *data, char *to_be_extended, int *indexes);

void			exec_program_create_fork(t_data *data, char *abs_path);
// void			exec_program_create_fork(t_data *data);
void			exec_error(t_data *data, int err, char *info, int exit);
bool			builtin_error_invalid_op(char *err);
bool			builtin_error(t_data *data, char *err);

bool			builtin_where(t_data *data);
// bool			err_msg(t_data *data, char *setv, int err);
char			last_char(char *str);
void			heredoc_set_flags(t_data *data);
int				redirs_pipes_fopen(t_data *data, char *filename, int flags);
int				enum_files(char *dir);
void			cd_set_oldpwd(t_data *data);
void			free_filenames(t_data *data);
void			init_prompt(t_data *data);
bool			syntax_check_pipes_redirs(t_data *data, char *cmd);

#endif
