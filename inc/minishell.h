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

//debug.c
void		dbg(t_data *data);

//exec/builtins/builtin_error.c
void		builtin_error(t_data *data, int err, char *info, int exit);

//exec/builtins/cd/cd_cleanup.c
void		free_cd(t_data *data);
void		init_cd(t_data *data, int *index_pwd);

//exec/builtins/cd/cd_utils.c
void		cd_root(t_data *data);
void		cd_set_oldpwd(t_data *data);

//exec/builtins/export/export.c
void		export_setv(t_data *data, char *setv);

//exec/builtins/export/export_utils.c
void		export_output(int len, char *name, char *val);
void		export_subshell_init(t_data *data);

//exec/exec.c
void		exec_create_fork(t_data *data, char *abs_path);

//exec/exec_pipes.c
void		exec_pipes(t_data *data);
void		exec_redirs_pipes(t_data *data);
void		exec_wait_for_childs(t_data *data);

//exec/exec_utils.c
void		exec_builtin_fork(t_data *data, bool flag);
void		exec_close_pipes(t_data *data);
void		exec_set_flags(t_data *data);

//minishell.c
void		clear_buffers(t_data *data);
void		init_prompt(t_data *data);

//parse/expand/expand_special_cases.c
void		expand_vars_exit(t_data *data);
void		expand_vars_not_exist(t_data *data);
void		expand_vars_shell(t_data *data);

//parse/expand/expand_utils.c
void		expand_vars_init(t_data *data, bool flag);
void		expand_vars_reset_flags(t_data *data);
void		expand_vars_rm_mod(t_data *data);
void		expand_vars_weird_special_case(t_data *data, bool flag);

//parse/parse_envp.c
void		init_envp(t_data *data);
void		parse_envp(t_data *data, char **envp);
void		parse_path(t_data *data);
void		sort_array(t_data *data);

//parse/parse_utils.c
void		realloc_envp(t_data *data, char *newv, int flag);

//parse/setup/setup.c
void		setup_reset_flags(t_data *data);

//parse/setup/setup_quote_escape.c
void		remove_backslashes(t_data *data, int i_arg);
void		remove_quotes(t_data *data, int i_arg);

//parse/setup/setup_quote_escape.c
void		remove_quotes_write_argv(t_data *data, int i_arg, char *tmp);

//parse/setup/setup_redir.c
void		free_filenames(t_data *data);

//parse/setup/setup_subshell.c
void		setup_subshell(t_data *data, char *cmd, int *i);

//parse/setup/setup_utils.c
void		remove_quotes_init(t_data *data);
void		setup_qflags(t_data *data, char *cmd, int *i);

//parse/wildcards/wildcards.c
void		get_all_names(t_data *data);

//pre_parse/heredoc/heredoc.c
void		rm_tmp_files(t_data *data);

//pre_parse/heredoc/heredoc_prompt.c
void		heredoc_prompt(t_data *data);

//pre_parse/heredoc/heredoc_utils.c
void		heredoc_free(t_data *data);
void		heredoc_init(t_data *data);
void		heredoc_wr_tmp_file(t_data *data);

//pre_parse/pre_parse_utils.c
void		pre_parse_check_ops(t_data *data, char **cmd, char op, int i);

//prompt/color/color_utils.c
void		color_cleanup(t_data *data);
void		color_help(void);

//prompt/prompt_history.c
void		history(t_data *data);

//prompt/prompt_prio.c
void		prio(t_data *data, char *cmd, int *i);

//prompt/prompt_utils.c
void		priorities(t_data *data, char **tmp_cmd, int *i);
bool		prompt_syntax_check(t_data *data, char **tmp_cmd);

//utils/cleanup.c
void		cleanup(t_data *data, int flag);
void		close_pipes(t_data *data);
void		free_argv(t_data *data, char **array);
void		ms_exit(t_status flag, int exit_status);

//utils/config.c
void		read_cfg(t_data *data);
void		read_cfg_history(t_data *data, char *read_buf);
//utils/init.c
void		init_vars(t_data *data);
void		open_pipes(t_data *data);

//utils/signal.c
void		heredoc_sig(void);
void		signal_handler(int sig, siginfo_t *info, void *context);
void		signal_handler_child(int sig, siginfo_t *info, void *context);
void		signal_handler_heredoc(int sig, siginfo_t *info, void *context);
void		signals(bool in_child);

//exec/builtins/cd/cd.c
bool		builtin_cd(t_data *data);

//exec/builtins/cd/cd_cleanup.c
bool		cd_err(t_data *data);
bool		cd_success(t_data *data, int i);

//exec/builtins/echo/echo.c
bool		builtin_echo(t_data *data);

//exec/builtins/env/env.c
bool		builtin_env(t_data *data);

//exec/builtins/exit/exit.c
bool		builtin_exit(t_data *data);

//exec/builtins/export/export.c
bool		builtin_export(t_data *data, char *setv);

//exec/builtins/export/export_subshell.c
bool		export_subshell(t_data *data, char *setv);

//exec/builtins/export/export_utils.c
bool		export_check_str(char *str);
bool		export_var(t_data *data, char *setv);

//exec/builtins/pwd/pwd.c
bool		builtin_pwd(t_data *data);

//exec/builtins/unset/unset.c
bool		builtin_unset(t_data *data);

//exec/builtins/where/where.c
bool		builtin_where(t_data *data);

//exec/exec.c
bool		exec_program(t_data *data);

//exec/exec_utils.c
bool		exec_check_path(char *cmd);

//exec/exec_utils.c
bool		exec_is_builtin(t_data *data);

//minishell.c
bool		count_pipes(t_data *data, char *cmd);

//parse/expand/expand_check_var.c
bool		expand_check_var_exists(t_data *data, char *var);

//parse/expand/expand_utils.c
bool		v_ex(t_data *data);

//parse/expand/expand_vars.c
bool		expand_vars(t_data *data);

//parse/setup/setup.c
bool		setup_argv(t_data *data, char *cmd, int *i);

//parse/setup/setup_modifiers.c
bool		setup_argv_is_and(t_data *data, int *i, int start_args);
bool		setup_argv_is_or(t_data *data, int *i, int start_args);
bool		setup_argv_is_pipe(t_data *data, int *i);
bool		setup_argv_is_redir_out(t_data *data, char *cmd, int *i);
bool		setup_argv_parse_arg(t_data *data, char *cmd, int i, bool end);
bool		setup_argv_set_redir_flags(t_data *data, char *cmd, int *i);

//parse/setup/setup_ops.c
bool		setup_andor(t_data *data, char *cmd, int *i, int start_args);
bool		setup_col(t_data *data, char *cmd, int *i);
bool		setup_esc(t_data *data, char *cmd, int *i);
bool		setup_pipe(t_data *data, char *cmd, int *i);
bool		setup_semicolon(t_data *data, char *cmd, int *i);

//parse/setup/setup_utils.c
bool		setup_alloc_argv(t_data *data, char *cmd);
bool		setup_all_filenames(t_data *data, int *i, char *cmd, int flag);

//parse/wildcards/wild_cases.c
bool		match_end(t_data *data, char *str, char *pattern);
bool		match_inbetween(t_data *data, char *str, char *pattern);
bool		match_start(t_data *data, char *str, char *pattern);

//pre_parse/syntax/syntax.c
bool		check_syntax(t_data *data, char *cmd);
bool		check_syntax_first_char(t_data *data, char *cmd);
bool		syntax_check_pipes_redirs(t_data *data, char *cmd);
bool		syntax_err_consecutive(t_data *data, char *cmd, int *i, int *j);

//pre_parse/syntax/syntax_error.c
bool		syntax_err(t_data *data, char *cmd);

//pre_parse/syntax/syntax_utils.c
bool		*err_msg(char *err_c);
bool		syntax_check_helper(char *cmd, char *ops, int i);
bool		syntax_check_iter(char *cmd, int *i);
bool		syntax_err_msg(t_data *data, char *ops, int i);

//prompt/color/color.c
bool		builtin_color(t_data *data, char *cfg);

//prompt/prompt_history.c
bool		builtin_history(t_data *data);

//utils/utils.c
bool		builtin(t_data *data);

//exec/exec.c
char		*exec_get_path(t_data *data, char *cmd);

//parse/expand/expand_get_var.c
char		*expand_get_var_content(t_data *data, char *var);

//parse/parse_utils.c
char		*strjoin_nl(char const *s1, char const *s2);

//parse/setup/setup_quote_escape.c
char		*remove_quotes_mid(t_data *data, int *i, int i_arg, char *argv);
char		*remove_quotes_start(t_data *data, int *i, int i_arg, char *tmp);

//parse/wildcards/wild_match.c
char		**match_files(t_data *data, char *to_be_extended, int *indexes);

//parse/wildcards/wild_utils.c
char		**realloc_argv(t_data *data, int argv_i, char *file, bool repl);
char		*delete_str(char *cmd, size_t start, size_t end);

//pre_parse/heredoc/heredoc.c
char		*handle_heredoc(t_data *data, char *cmd);

//pre_parse/heredoc/heredoc_utils.c
char		*heredoc_delim(char *cmd, int i, int j);

//pre_parse/heredoc/heredoc_vars.c
char		*heredoc_vars(t_data *data, char *line);

//pre_parse/pre_parse.c
char		*pre_parse(t_data *data, char *cmd);
char		*pre_parse_rm_delim(t_data *data, char *cmd, int start, int end);
char		*pre_parse_insert_space(t_data *data, char *cmd, int index);
char		*pre_parse_skip_d(t_data *data, char *cmd, char delim);

//pre_parse/syntax/syntax_utils.c
char		*syntax_err_type(t_data *data, char c, int exit_status, int flag);

//prompt/prompt_prio.c
char		last_char(char *str);

//utils/utils.c
char		*strmv(char *new);

//exec/exec_pipes.c
int			exec_redirs_pipes_fopen(t_data *data, char **filename, int flags);

//minishell.c
int			main(int argc, char **argv, char **envp);

//parse/parse_utils.c
int			strcmp_alnum(const char *s1, const char *s2);

//parse/setup/setup_redir.c
bool		setup_redir(t_data *data, char *cmd, int *i);

//parse/wildcards/wild_enum_files.c
int			enum_files(char *dir);

//parse/wildcards/wild_utils.c
int			*get_indexes(char *str);
int			len_argv(char **argv);
int			num_wildcards(char *str);

//prompt/prompt.c
int			prompt(t_data *data, char *cmd, int flag);

//utils/config.c
int			strdiff(const char *s1, const char *s2);

//utils/utils.c
int			isidentifier(int c);
int			isnumeric(int c);

//exec/builtins/exit/exit.c
long long	ms_atoll(t_data *data, const char *str);

//exec/builtins/cd/cd_utils.c
size_t		cd_find_pwd(t_data *data);

//parse/parse_utils.c
size_t		strlen_path(const char *c);
size_t		strlen_var(const char *c);

//utils/init.c
t_data		*allocate_mem(void);

#endif
