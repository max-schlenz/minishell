/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:29:23 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 16:47:44 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

# include <minishell.h>

typedef struct s_subshell
{
	char	*buf;
	char	*vcontent;
	char	*vname;
	char	*vname_eq;
	char	*var_tmp;
	char	*var;
	int		pipe[2];
}	t_subshell;

typedef struct s_wcmatch
{
	int		word_len;
	int		prev_index;
	char	*sub_word;
	int		i;
	int		left;

}	t_wcmatch;

typedef struct s_exit
{
	long long	max;
	long long	i;
	long long	r;
	long long	neg;
}	t_exit;

typedef struct s_pparse
{
	int		i;
	int		j;
	bool	f_dquote;
	bool	f_squote;
	char	*ret;
}	t_pparse;

typedef struct s_variable
{
	char	*name;
	char	*value;
	char	*pre;
	char	*pre_and_value;
	char	*rest;
	int		i_arg;
	size_t	i_char;
	bool	f_dquote;
	bool	f_squote;
	bool	f_esc;
}	t_variable;

typedef struct s_export
{
	int		index_arg;
	bool	free_set;
	bool	set;
	char	*err;
	int		index_envp;
	size_t	len;
}	t_export;

typedef struct s_echo
{
	int		index_char;
	bool	f_space;
	bool	f_fc;
}	t_echo;

typedef struct s_cd
{
	char	*path;
	char	*path_tmp_bs;
	char	*path_tmp;
	char	*path_tmp2;
	char	*new_pwd_tmp;
	bool	oldpwd_err;
}	t_cd;

typedef struct s_heredoc
{
	char	*cmd_begin;
	char	*cmd;
	char	*cmd_end;
	char	*cmd_redir;
	char	*cmd_redir_file;
	char	*cmd_done;
	char	*cmd_done1;
	char	*delim;
	char	*hd_tmp;
	char	*hd_tmp_i;
	bool	andor;
	bool	hd;
	bool	quote;
	bool	sig;
}	t_heredoc;

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
	bool	f_rmq;
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
	bool	prompt_exec;
	bool	and;
	bool	or;
	bool	pipe;
	bool	debug;
	bool	bracket;
	bool	prio;
	bool	noenv;
	bool	exit_code_of;
	bool	macos;
	bool	rndcl;
	bool	f_dquote;
	bool	f_squote;
	bool	f_esc;
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
	char		clr[3];
	int			counter_env;
	int			counter_pipes;
	int			fd_i;
	char		*prompt;
	char		*prompt_cl2;
	char		*file_name;
	char		*file_name2;
	char		*file_name_append;
	char		*last_cmd;
	char		**wc_dir_content;
	int			exit_code;
	long long	exit_status;
	int			fd;
	int			fd_stdout;
	int			fd_stdin;
	int			mscfg;
	int			heredoc_index;
	int			arr[4][4];
	pid_t		pid;
	FILE		*debug;
	t_heredoc	hdoc;
	t_export	export;
	t_parser	parser;
	t_color		color;
	t_rmq		rmq;
	t_cd		cd;
	t_echo		echo;
	t_variable	var;
	t_pparse	pparse;
	t_exit		exit;
	t_wcmatch	wcmatch;
	t_subshell	subshell;
	t_flags		*flags;
	t_pipes		*pipes;
}	t_data;

typedef enum s_status
{
	SUCCESS,
	E_MEM,
	E_FORK,
	E_EXECVE,
	E_RW,
}	t_status;

#endif