/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 09:47:32 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/06 14:15:31 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	clear_buffers(t_data *data)
{
	// free_array(data->argv);
	// free(data->argv);
	close_pipes(data);
	data->flags->pipe = 0;
}

static void	init_prompt(t_data *data)
{
	data->flags->error = false;
	data->flags->pipe = false;
	data->flags->redir_out = false;
	data->flags->redir_out_append = false;
	data->flags->redir_in = false;
	data->flags->redir_in_delim = false;
	data->flags->and = false;
	data->flags->or = false;
	data->counter_pipes = 0;
	data->fd_i = 0;
}

static bool count_pipes(t_data *data)
{
	int i;

	i = 0;
	while (data->cmd[i])
	{
		if (!ft_strncmp(data->cmd + i, "||", 2))
			i += 2;
		if (data->cmd[i] && data->cmd[i] == '|')
			data->counter_pipes++;
		i++;
	}
	if (i)
		return (true);
	return (false);
}

static char	*cleanup_str_single(char *cmd)
{
	char **tmp;
	char *tmp_trim;
	char *tmp_trim_space_s;
	char *tmp_trim_space_e;
	char *tmp_trim_op;
	char **tmp_trim_arr;

	char *tmp_join;
	char *ret;
	tmp = split_single(cmd, '|');
	int arr_size = 0;
	while (tmp[arr_size])
		arr_size++;
	
	// int ll = 0;

	// while (tmp[ll])
	// 	printf("%s\n", tmp[ll++]);

	// exit(0);
	// printf("%d\n", arr_size);
	tmp_trim_arr = ft_calloc(arr_size + 1, sizeof(char *));

	// tmp_trim_op = ft_strjoin(tmp[0], "|");
	// tmp_trim_arr[0] = ft_strdup(tmp_trim_op);
	int i = 0;
	while (tmp[i])
	{
		tmp_trim = ft_strtrim(tmp[i], " ");
		tmp_trim_space_s = ft_strjoin(" ", tmp_trim);
		if (!tmp[i + 1])
			tmp_trim_arr[i++] = ft_strdup(tmp_trim_space_s);
		else
		{
			tmp_trim_space_e = ft_strjoin(tmp_trim_space_s, " |");
			tmp_trim_arr[i++] = ft_strdup(tmp_trim_space_e);
		}
	}

	int k = 0;
	tmp_join = NULL;
	while (tmp_trim_arr[k])
	{
		tmp_join = ft_strjoin_dup(tmp_join, tmp_trim_arr[k]);
		free (tmp_trim_arr[k++]);
	}
	free (tmp_trim_arr);
	ret = ft_strtrim(tmp_join, "| ");
	return ret;
}

// static char	*cleanup_str_double_pipes(char *cmd)
// {
// 	char **tmp;
// 	char *tmp_trim;
// 	char *tmp_trim_space_s;
// 	char *tmp_trim_space_e;
// 	char *tmp_trim_op;
// 	char **tmp_trim_arr;

// 	char *tmp_join;
// 	char *ret;

// 	char *ops;
// 	ops = ft_strdup("|&");
// 	while (*ops)
// 	{
// 		int arr_size = 0;
// 		int i = 0;
// 		int k = 0;
// 		if (*ops == '|')
// 			tmp = split_double_pipes(cmd, '|');
// 		else
// 			tmp = split_double_and(cmd, '&');
// 		while (tmp[arr_size])
// 			arr_size++;
// 		// printf("%d\n", arr_size);
// 		tmp_trim_arr = ft_calloc(arr_size + 1, sizeof(char *));

// 		// tmp_trim_op = ft_strjoin(tmp[0], "|");
// 		// tmp_trim_arr[0] = ft_strdup(tmp_trim_op);
// 		while (tmp[i])
// 		{
// 			tmp_trim = ft_strtrim(tmp[i], " ");
// 			tmp_trim_space_s = ft_strjoin(" ", tmp_trim);
// 			if (!tmp[i + 1])
// 				tmp_trim_arr[i++] = ft_strdup(tmp_trim_space_s);
// 			else
// 			{
// 				if (*ops == '|')
// 					tmp_trim_space_e = ft_strjoin(tmp_trim_space_s, " ||");
// 				else
// 					tmp_trim_space_e = ft_strjoin(tmp_trim_space_s, " &&");
// 				tmp_trim_arr[i++] = ft_strdup(tmp_trim_space_e);
// 			}
// 		}
// 		tmp_join = NULL;
// 		while (tmp_trim_arr[k])
// 		{
// 			tmp_join = ft_strjoin_dup(tmp_join, tmp_trim_arr[k]);
// 			free (tmp_trim_arr[k++]);
// 		}
// 		free (tmp_trim_arr);
// 		ret = ft_strtrim(tmp_join, "| ");
// 		ops++;
// 	}
// 	return ret;
// }



static char	*cleanup_str_double_and(char *cmd)
{
	char **tmp;
	char *tmp_trim;
	char *tmp_trim_space_s;
	char *tmp_trim_space_e;
	char *tmp_trim_op;
	char **tmp_trim_arr;

	char *tmp_join;
	char *ret;
	tmp = split_double_and(cmd, '&');
	int arr_size = 0;
	while (tmp[arr_size])
		arr_size++;
	// printf("%d\n", arr_size);
	tmp_trim_arr = ft_calloc(arr_size + 1, sizeof(char *));

	// tmp_trim_op = ft_strjoin(tmp[0], "|");
	// tmp_trim_arr[0] = ft_strdup(tmp_trim_op);
	int i = 0;
	while (tmp[i])
	{
		tmp_trim = ft_strtrim(tmp[i], " ");
		tmp_trim_space_s = ft_strjoin(" ", tmp_trim);
		if (!tmp[i + 1])
			tmp_trim_arr[i++] = ft_strdup(tmp_trim_space_s);
		else
		{
			tmp_trim_space_e = ft_strjoin(tmp_trim_space_s, " &&");
			tmp_trim_arr[i++] = ft_strdup(tmp_trim_space_e);
		}
	}

	int k = 0;
	tmp_join = NULL;
	while (tmp_trim_arr[k])
	{
		tmp_join = ft_strjoin_dup(tmp_join, tmp_trim_arr[k]);
		free (tmp_trim_arr[k++]);
	}
	free (tmp_trim_arr);
	ret = ft_strtrim(tmp_join, "& ");
	return ret;
}

static char *add_space(char *cmd, int index)
{
	size_t	len;
	char	*ret;
	int		i;
	int		j;

	len = ft_strlen(cmd) + 1;
	ret = (char *)malloc(sizeof(char) * len + 1);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (i == index)
		{
			j++;
			ret[j] = ' ';
		}
		ret[j] = cmd[i];
		i++;
		j++;
	}
	return (ret);
}

static char *add_spaces(char *cmd)
{
	int	i;
	char *ops;

 	ops = ft_strdup("|&><");
	i = 0;
	while (*ops)
	{
		i = 0;
		while (cmd[i])
		{
			if (cmd[i + 1] && cmd[i] != ' ' && cmd[i + 1] == *ops)
			{
				cmd = add_space(cmd, i);
			}
				
			if (cmd[i + 1] && cmd[i + 1] != ' ' && cmd[i + 1] != *ops && cmd[i] == *ops)
			{
				cmd = add_space(cmd, i + 1);
			}
			printf("in here %s\n", cmd);
			i++;
		}
		ops++;
	}
	return (cmd);
}

static void	history(t_data *data)
{
	if (strdiff(data->cmd, data->last_cmd))
	{
		add_history(data->cmd);
		write(data->mscfg, "HISTORY=", 8);
		write(data->mscfg, data->cmd, ft_strlen(data->cmd));
		write(data->mscfg, "\n", 1);
	}
	data->last_cmd = ft_strdup(data->cmd);
}
static bool	check_syntax(t_data *data)
{
	int		i;
	char	*ops_supported;
	char	*ops_unsupported;
	bool	quote;

	ops_supported = ft_strdup("|&><");
	ops_unsupported = ft_strdup("{");
	while (*ops_supported)
	{
		i = 0;
		while (data->cmd[i])
		{
			if (data->cmd[i] == '\'' || data->cmd[i] == '\"')
			{
				i++;
				while (data->cmd[i + 2])
				{
					if (data->cmd[i] == '\'' || data->cmd[i++] == '\"')
						break ;
					i++;
				}
			}
			if	(data->cmd[i]
			&&	data->cmd[i + 1]
			&&	data->cmd[i + 2]
			&&	data->cmd[i] == *ops_supported
			&&	data->cmd[i + 1] == *ops_supported
			&&	data->cmd[i + 2] == *ops_supported)
			{
				printf("Syntax error: '%c' [%d]\n", *ops_supported, i + 3);
				return (false);
			}
			i++;
		}
		ops_supported++;
	}
	while (*ops_unsupported)
	{
		i = 0;
		while (data->cmd[i])
		{
			if (data->cmd[i] == '\'' || data->cmd[i] == '\"')
			{
				i++;
				while (data->cmd[i])
				{
					if (data->cmd[i] == '\'' || data->cmd[i++] == '\"')
						break ;
					i++;
				}
			}
			if	(data->cmd[i] == *ops_unsupported)
			{
				printf("Syntax error: '%c' [%d] unsupported operation\n", *ops_unsupported, i + 3);
				return (false);
			}
			i++;
		}
		ops_unsupported++;
	}
	return (true);
}

static void	prompt(t_data *data)
{
	bool	left;
	char	*cmd;

	left = true;
	data->cmd = readline(data->prompt);
	if (!data->cmd)
		data->cmd = "exit";
	else if (data->cmd[0] && data->cmd[0] != '\n')
	{
		history(data);
		if (count_pipes(data))
			open_pipes(data);
		if (!check_syntax(data))
			return ;
		// data->cmd = cleanup_str_double_pipes(data->cmd);
		// data->cmd = cleanup_str_double_and(data->cmd);
		//data->cmd = cleanup_str_single(data->cmd);
		data->cmd = add_spaces(data->cmd);
	}
	while (data->cmd && data->cmd[0] != '\0')
	{
		while (*data->cmd == ' ')
			data->cmd++;
		printf("%s\n", data->cmd);
		exit(0);
		data->cmd = split_quotes(data, data->cmd);
		// if (*data->cmd == ';')
		// 	data->cmd++;
		if (data->flags->error || !data->argv[0])
			continue;
		if ((left)
		||	(!data->flags->and && !data->flags->or) 
		||	(data->flags->and && !data->exit_status) 
		||	(data->flags->or && data->exit_status))
		{
			if (!builtin_environment(data))
				exec_program(data);
		}
		free_array(data->argv);
		free(data->argv);
		left = !left;
	}
	data->flags->and = false;
	data->flags->or = false;
}

static void	signals()
{
	struct sigaction	sa;
	
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	// signal(SIGQUIT, SIG_IGN);
}

static void create_config(t_data *data)
{
	int fd;

	fd = open(".mscfg", O_CREAT | O_RDWR, 0644);
	write(fd, "COLOR=bc\n", 9);
	close (fd);
}

static void	read_config(t_data *data)
{
	char *read_buf;
	char *history;

	if (access(".mscfg", F_OK))
		create_config(data);
	if (!access(".mscfg", F_OK))
	{
		data->mscfg = open(".mscfg", O_RDWR | O_APPEND, 0644);
		read_buf = ft_strdup("42");
		while (read_buf != NULL)
		{
			free(read_buf);
			read_buf = get_next_line(data->mscfg);
			if (read_buf && !strcmp_alnum(read_buf, "COLOR"))
				builtin_color(data, read_buf + strlen_var(read_buf) + 1);
			if (read_buf && !strcmp_alnum(read_buf, "HISTORY"))
			{
				history = ft_strtrim(read_buf + 8, "\n");
				add_history(history);
				data->last_cmd = ft_strdup(history);
			}
		}
		return ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data		*data;

	data = allocate_mem();
	signals();
	init_vars(data, argv);
	read_config(data);
	parse_envp(data, envp);
	while (1)
	{
		init_prompt(data);
		prompt(data);
		clear_buffers(data);
	}
}
