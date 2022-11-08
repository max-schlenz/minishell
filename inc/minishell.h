/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 10:56:28 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/08 18:09:10 by mschlenz         ###   ########.fr       */
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

//src/exec/builtins/builtin_error.c
void			builtin_error(t_data *data, int err, char *info, int exit);

//src/exec/builtins/cd/cd.c
bool			builtin_cd(t_data *data);

//src/exec/builtins/cd/cd_cleanup.c
bool			cd_err(t_data *data);
bool			cd_success(t_data *data, int i);
void			free_cd(t_data *data);
void			init_cd(t_data *data, int *index_pwd);

//src/exec/builtins/cd/cd_utils.c
size_t			cd_find_pwd(t_data *data);
void			cd_root(t_data *data);
void			cd_set_oldpwd(t_data *data);

//src/exec/builtins/echo/echo.c
bool			builtin_echo(t_data *data);

//src/exec/builtins/env/env.c
bool			builtin_env(t_data *data);

//src/exec/builtins/exit/exit.c
bool			builtin_exit(t_data *data);
long long		ms_atoll(t_data *data, const char *str);

//src/exec/builtins/export/export.c
bool			builtin_export(t_data *data, char *setv);
void			export_setv(t_data *data, char *setv);

//src/exec/builtins/export/export_utils.c
bool			export_check_str(char *str);
bool			export_var(t_data *data, char *setv);
void			export_output(int len, char *name, char *val);

//src/exec/builtins/pwd/pwd.c
bool			builtin_pwd(t_data *data);

//src/exec/builtins/unset/unset.c
bool			builtin_unset(t_data *data);

//src/exec/builtins/where/where.c
bool			builtin_where(t_data *data);

//src/exec/exec.c
bool			exec_program(t_data *data);
char			*exec_get_path(t_data *data, char *cmd);
void			exec_create_fork(t_data *data, char *abs_path);

//src/exec/exec_pipes.c
int				exec_redirs_pipes_fopen(t_data *data, char **file, int flags);
void			exec_pipes(t_data *data);
void			exec_redirs_pipes(t_data *data);
void			exec_wait_for_childs(t_data *data);

//src/exec/exec_utils.c
bool			exec_check_path(char *cmd);
bool			exec_is_builtin(t_data *data);
void			exec_builtin_fork(t_data *data, bool flag);
void			exec_close_pipes(t_data *data);
void			exec_set_flags(t_data *data);

//src/minishell.c
bool			count_pipes(t_data *data, char *cmd);
int				main(int argc, char **argv, char **envp);
void			clear_buffers(t_data *data);
void			init_prompt(t_data *data, bool execution);

//src/parse/expand/expand_check_var.c
bool			expand_check_var_exists(t_data *data, char *var);

//src/parse/expand/expand_get_var.c
char			*expand_get_var_content(t_data *data, char *var);

//src/parse/expand/expand_special_cases.c
void			expand_vars_exit(t_data *data);
void			expand_vars_not_exist(t_data *data);
void			expand_vars_shell(t_data *data);

//src/parse/expand/expand_utils.c
bool			expand_check_var_exists_util(t_data *data);
void			expand_vars_init(t_data *data, bool flag);
void			expand_vars_reset_flags(t_data *data);
void			expand_vars_rm_mod(t_data *data);

//src/parse/expand/expand_vars.c
bool			expand_vars(t_data *data);

//src/parse/parse_envp.c
void			init_envp(t_data *data);
void			parse_envp(t_data *data, char **envp);
void			parse_path(t_data *data);
void			sort_array(t_data *data);

//src/parse/parse_utils.c
char			*strjoin_nl(char const *s1, char const *s2);
int				strcmp_alnum(const char *s1, const char *s2);
size_t			strlen_path(const char *c);
size_t			strlen_var(const char *c);
void			realloc_envp(t_data *data, char *newv, int flag);

//src/parse/setup/setup.c
bool			setup_alloc_argv(t_data *data, char *cmd);
bool			setup_argv(t_data *data, char *cmd, int *i);
void			setup_def(t_data *data, char *cmd, int *i);

//src/parse/setup/setup_modifiers.c
bool			setup_argv_parse_and(t_data *data, int *i, int start_args);
bool			setup_argv_parse_or(t_data *data, int *i, int start_args);
bool			setup_argv_parse_pipe(t_data *data, int *i);
bool			setup_argv_set_redir_flags(t_data *data, char *cmd, int *i);
bool			setup_argv_write_arg(t_data *data, char *cmd, int i, bool end);

//src/parse/setup/setup_ops.c
bool			andor(t_data *data, char *cmd, int *i, int start_args);
bool			col(t_data *data, char *cmd, int *i);
bool			escape(t_data *data, char *cmd, int *i);
bool			pipe_(t_data *data, char *cmd, int *i);
bool			semicolon(t_data *data, char *cmd, int *i);

//src/parse/setup/setup_quote_escape.c
void			remove_backslashes(t_data *data, int i_arg);
void			remove_quotes(t_data *data, int i_arg);
void			remove_quotes_write_argv(t_data *data, int i_arg, char *tmp);

//src/parse/setup/setup_redir.c
bool			redir(t_data *data, char *cmd, int *i);
void			free_filenames(t_data *data);

//src/parse/setup/setup_utils.c
bool			setup_all_filenames(t_data *data, int *i, char *cmd, int flag);
int				next_cmd(int *i, char *cmd);
void			quote_flags(t_data *data, char *cmd, int *i);
void			remove_quotes_init(t_data *data);
void			setup_reset_flags(t_data *data);

//src/parse/wildcards/wild_cases.c
bool			match_end(t_data *data, char *str, char *pattern);
bool			match_inbetween(t_data *data, char *str, char *pattern);
bool			match_start(t_data *data, char *str, char *pattern);

//src/parse/wildcards/wild_enum_files.c
int				enum_files(char *dir);

//src/parse/wildcards/wild_match.c
char			**match_files(t_data *data, char *to_be_extended, int *indexes);

//src/parse/wildcards/wild_utils.c
char			**realloc_argv(t_data *data, int argv_i, char *file, bool repl);
char			*delete_str(t_data *data, char *cmd, size_t start, size_t end);
int				*get_indexes(t_data *data, char *str);
int				len_argv(char **argv);
int				num_wildcards(char *str);

//src/parse/wildcards/wildcards.c
void			resolve_wildcards(t_data *data);

//src/pre_parse/heredoc/heredoc.c
char			*handle_heredoc(t_data *data, char *cmd);
void			rm_tmp_files(t_data *data);

//src/pre_parse/heredoc/heredoc_prompt.c
void			heredoc_prompt(t_data *data);

//src/pre_parse/heredoc/heredoc_utils.c
char			*heredoc_delim(char *cmd, int i, int j);
void			heredoc_free(t_data *data);
void			heredoc_init(t_data *data);
void			heredoc_wr_tmp_file(t_data *data);

//src/pre_parse/heredoc/heredoc_vars.c
char			*heredoc_vars(t_data *data, char *line);

//src/pre_parse/pre_parse.c
char			*pre_parse(t_data *data, char *cmd);
char			*pre_parse_insert_space(t_data *data, char *cmd, int index);
char			*pre_parse_skip_d(t_data *data, char *cmd, char delim);

//src/pre_parse/pre_parse_quotes.c
char			*strrmstr(char *str, char *needle);

//src/pre_parse/pre_parse_utils.c
char			*remove_quotes_spaces(t_data *data, char *cmd);
void			pre_parse_check_ops(t_data *data, char **cmd, char op, int i);

//src/pre_parse/syntax/syntax.c
bool			check_syntax(t_data *data, char *cmd);
bool			check_syntax_first_char(t_data *data, char *cmd);
bool			syntax_check_pipes_redirs(t_data *data, char *cmd);
bool			syntax_err_consecutive(t_data *data, char *cmd, int *i, int *j);

//src/pre_parse/syntax/syntax_error.c
bool			syntax_err(t_data *data, char *cmd);

//src/pre_parse/syntax/syntax_utils.c
bool			*err_msg(char *err_c);
bool			syntax_check_helper(char *cmd, char *ops, int i);
bool			syntax_check_iter(char *cmd, int *i);
bool			syntax_err_msg(t_data *data, char *ops, int i);
char			*syntax_err_type(t_data *data, char c, int exit, int flag);

//src/prompt/color/color.c
bool			builtin_color(t_data *data, char *cfg);

//src/prompt/color/color_utils.c
void			color_cleanup(t_data *data);
void			color_help(void);

//src/prompt/prompt.c
int				prompt(t_data *data, char *cmd, int flag);

//src/prompt/prompt_history.c
bool			builtin_history(t_data *data);
void			history(t_data *data);

//src/prompt/prompt_prio.c
char			last_char(char *str);
void			prio(t_data *data, char *cmd, int *i);

//src/prompt/prompt_utils.c
bool			prompt_syntax_check(t_data *data, char **tmp_cmd);
void			priorities(t_data *data, char **tmp_cmd, int *i);

//src/utils/cleanup.c
void			cleanup(t_data *data, int flag);
void			close_pipes(t_data *data);
void			free_argv(t_data *data, char **array);
void			ms_exit(t_status flag, int exit_status);

//src/utils/config.c
int				strdiff(const char *s1, const char *s2);
void			read_cfg(t_data *data);
void			read_cfg_history(t_data *data, char *read_buf);

//src/utils/init.c
t_data			*allocate_mem(void);
void			init_vars(t_data *data);

//src/utils/signal.c
void			heredoc_sig(void);
void			signal_handler(int sig, siginfo_t *info, void *context);
void			signal_handler_child(int sig, siginfo_t *info, void *context);
void			signal_handler_heredoc(int sig, siginfo_t *info, void *context);
void			signals(bool in_child);

//src/utils/utils.c
bool			builtin(t_data *data);
char			*strmv(char *new);
int				isidentifier(int c);
int				isnumeric(int c);
void			dbg(t_data *data);

#endif
