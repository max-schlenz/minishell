/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/03 18:46:30 by tdehne            #+#    #+#             */
/*   Updated: 2022/09/19 15:33:45 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	builtin_cd(t_data *data)
{
	char	*path;
	char	*path_tmp_bs;
	char	*path_tmp;
	char	*path_tmp2;
	char	*new_pwd_tmp;
	int		i;
	int		j;
	int		k;
	
	i = 0;
	k = 0;
	new_pwd_tmp = NULL;
	while (data->envp[i] && strcmp_alnum(data->envp[i], "PWD"))
		i++;
	if (data->argv[1] && !ft_strncmp(data->argv[1], ".", 2))
		return (true);
	if (data->argv[1])
	{
		path_tmp2 = ft_strtrim(data->argv[1], " ");
		if (!ft_strncmp(data->argv[1], "-", 2))
		{
			k = ft_strlen(data->envp[i] + 4);
			while (k && data->envp[i][k] != '/')
				k--;
			path_tmp = ft_strdup("");
			path_tmp_bs = ft_strdup("");
			path = ft_substr(data->envp[i], 4, k - 4);
			new_pwd_tmp = ft_strjoin("PWD=", path);
			free (path_tmp_bs);
			free (path_tmp);
		}
		else if (data->argv[1][0] != '/')
		{
			path_tmp = ft_strdup(data->envp[i] + 4);
			path_tmp_bs = ft_strjoin(path_tmp, "/");
			path = ft_strjoin(path_tmp_bs, path_tmp2);
			free (path_tmp_bs);
			free (path_tmp);
		}
		else
		{
			new_pwd_tmp = ft_strjoin("PWD=", path_tmp2);
			path = ft_strdup(path_tmp2);
		}
		free (path_tmp2);
		if (!access(path, F_OK))
		{
			chdir(path);
			free (path);
			if (ft_strncmp(data->argv[1], "..", 3) && (!new_pwd_tmp))
			{
				if (data->envp[i][ft_strlen(data->envp[i]) - 1] != '/')
					path = ft_strjoin("/", data->argv[1]);
				else
					path = ft_strdup(data->argv[1]);
				new_pwd_tmp = ft_strjoin(data->envp[i], path);
				free (path);
			}
			else if (!new_pwd_tmp)
			{
				j = ft_strlen(data->envp[i]) - 1;
				while (data->envp[i][j] && data->envp[i][j] != '/')
					j--;
				new_pwd_tmp = ft_substr(data->envp[i], 0, j);
			}
		}
		else
		{
			perror("Error");
			free (path);
			free (new_pwd_tmp);
			data->exit_status = 1;
			return (true);
		}
	}
	else
	{
		chdir("/root");
		new_pwd_tmp = ft_strdup("PWD=/root");
	}
	free (data->envp[i]);
	data->envp[i] = ft_strdup(new_pwd_tmp);
	free (new_pwd_tmp);
	data->exit_status = 0;
	return (true);
}

bool	builtin_echo(t_data *data)
{
	int 	i = 1;
	char	*cmd_trim;
	bool	echo_n;
	bool	f_squote = false;
	bool	f_dquote = false;
	bool	f_space = false;
	int j = 0;
	int k = 0;
	int l = 0;

	echo_n = false;
	if (data->argv[i] && !ft_strncmp(data->argv[i], "-n", 2))
	{
		j = 0;
		k = 0;
		while (data->argv[i][j])
		{
			if (data->argv[i][j] == '-')
				k++;
			j++;
		}
		if (k == 1)
		{
			echo_n = true;
			i++;
		}
	}
	while (data->argv[i])
	{
		while (echo_n && !ft_strncmp(data->argv[i], "-n", 2))
			i++;
		while (data->argv[i][l])
		{
			if (data->argv[i][l] != '\\')
				printf("%c", data->argv[i][l]);
			l++;
		}
		l = 0;
		i++;
		if (data->argv[i] && ft_strlen(data->argv[i]) > 0 && (!(echo_n && i < 3)))
			printf(" ");
	}
	if (!echo_n)
		printf("\n");
	data->exit_status = 0;
	return (true);
}

static bool	export_check_str(t_data *data, char *str)
{
	bool	check1;
	bool	check2;
	int		i;

	i = 0;
	check1 = false;
	check2 = false;
	while (str[i] && (str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || str[i] == '_' || str[i] == '+')
		i++;
	if (str[i] && str[i] != '=')
		return (false);
	i -= 2;
	while (str[i] && str[i] != '+')
		i--;
	if (i >= 0)
		return (false);
	return (true);
}

bool	builtin_export(t_data *data, char *setv)
{
	int		i;
	size_t	len;
	int		len_name;
	int		len_content;
	char	*str_name;
	char	*str_content;
	bool	set;

	i = 0;
	set = false;
	if (!setv)
		setv = data->argv[1];
	if (setv && setv[0] == '-')
	{
		write(2, "Error: export: options not supported\n", 3);
		data->exit_status = 2;
		return (true);
	}
	if (setv && !export_check_str(data, setv))
	{
		data->exit_status = 1;
		return (true);
	}
	if (setv && isidentifier(setv[0]))
	{
		len = strlen_var(setv);
		if (setv[len] == '=')
		{
			if (len < ft_strlen(setv) - 1)
			{
				while (data->envp[i])
				{
					if (!ft_strncmp(data->envp[i], setv, len))
					{
						free(data->envp[i]);
						data->envp[i] = ft_strdup(setv);
						set = true;
					}
					i++;
				}
				if (!set && !data->envp[i])
					realloc_envp(data, setv, 1);
				sort_array(data);
				parse_path(data);
			}
		}
		data->exit_status = 0;
		return (true);
	}
	else if (setv)
	{
		data->exit_status = 1;
		// printf("export: %s is not a valid identifier\n", setv);
		write(2, "export: not a valid identifier\n", 32);
	}
	if (!setv)
	{
		pid_t pid;
		pid = fork();
		if (pid == 0)
		{
			redirs_pipes(data);
			i = 0;
			while (data->envp[i])
			{
				len_name = strlen_var(data->envp[i]);
				len_content = ft_strlen(data->envp[i]) - len_name;
				str_name = ft_substr(data->envp[i], 0, len_name);
				str_content = ft_substr(data->envp[i], len_name + 1, len_content);
				printf("declare -x %s=\"%s\"\n", str_name, str_content);
				free (str_name);
				free (str_content);
				i++;
			}
			exit(0);
		}
		waitpid(pid, &data->exit_code, 0);
		reset_pipes_flags(data);
		data->exit_status = 0;
	}
	return (true);
}

bool	builtin_env(t_data *data)
{
	int	i;

	i = 0;
	if (!data->argv[1])
	{
		while (data->envp[i])
			printf("%s\n", data->envp[i++]);
		data->exit_status = 0;
	}
	else
		printf("syntax error\n");
	return (true);
}

bool	builtin_pwd(t_data *data)
{
	int	i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PWD", 3))
		i++;
	printf("%s\n", data->envp[i] + 4);
	data->exit_status = 0;
	return (true);
}

bool	builtin_unset(t_data *data)
{
	int i;

	i = 0;
	if (data->argv[1])
	{
		while (data->envp[i])
		{
			if (!ft_strncmp(data->envp[i], data->argv[1], ft_strlen(data->argv[1])))
				break;
			i++;
		}
		while (i < data->counter_env - 1)
		{
			free(data->envp[i]);
			data->envp[i] = ft_strdup(data->envp[i + 1]);
			i++;
		}
		free(data->envp[i]);
		data->envp[i] = NULL;
		sort_array(data);
		data->exit_status = 0;
		return (true);
	}
	data->exit_status = 0;
	return (true);
}

bool	builtin_color(t_data *data, char *cfg)
{
	char	*prompt[4];
	char	*prompt_tmp;
	char	*cfg_str_pf;
	char	*cfg_str_cl;
	char	*cfg_str;
	char	*prompt_tmp2;
	char	*code[2];
	int		fd;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	prompt[1] = ft_strdup("mini");
	prompt[3] = ft_strdup("shell\x01\033[0;1m\x02 #\x01\033[0m\x02  ");
	if (!cfg)
	{
		printf("change the color of your prompt!\n\n \
			w - \033[0mwhite\033[0m\n \
			r - \033[31;1mred\033[0m\n \
			g - \033[32;1mgreen\033[0m\n \
			y - \033[33;1myellow\033[0m\n \
			b - \033[34;1mblue\033[0m\n \
			m - \033[35;1mmagenta\033[0m\n \
			c - \033[36;1mcyan\033[0m\n\n");
	}
	while (i < 2)
	{
		if (!cfg)
		{
			if (!i)
				code[i] = readline("enter color code 1: ");
			else
				code[i] = readline("enter color code 2: ");
		}
		else
			code[i] = ft_substr(cfg, i, 1);
		if (code[i])
		{
			if (!ft_strncmp(code[i], "white", 6) || !ft_strncmp(code[i], "w", 2))
				prompt[j] = ft_strdup(WHITE);
			else if (!ft_strncmp(code[i], "red", 4) || !ft_strncmp(code[i], "r", 2))
				prompt[j] = ft_strdup(RED);
			else if (!ft_strncmp(code[i], "green", 6) || !ft_strncmp(code[i], "g", 2))
				prompt[j] = ft_strdup(GREEN);
			else if (!ft_strncmp(code[i], "yellow", 7) || !ft_strncmp(code[i], "y", 2))
				prompt[j] = ft_strdup(YELLOW);
			else if (!ft_strncmp(code[i], "blue", 5) || !ft_strncmp(code[i], "b", 2))
				prompt[j] = ft_strdup(BLUE);
			else if (!ft_strncmp(code[i], "magenta", 8) || !ft_strncmp(code[i], "m", 2))
				prompt[j] = ft_strdup(MAGENTA);
			else if (!ft_strncmp(code[i], "cyan", 5) || !ft_strncmp(code[i], "c", 2))
				prompt[j] = ft_strdup(CYAN);
			else
				continue;
		}
		j = 2;
		i++;
	}
	fd = open(".mscfg", O_RDWR, 0644);
	cfg_str_pf = ft_strdup("COLOR=");
	cfg_str_cl = ft_strjoin(cfg_str_pf, code[0]);
	cfg_str = ft_strjoin(cfg_str_cl, code[1]);
	write(fd, cfg_str, ft_strlen(cfg_str));
	prompt_tmp = ft_strjoin(prompt[0], prompt[1]);
	prompt_tmp2 = ft_strjoin(prompt[2], prompt[3]);
	free (data->prompt);
	data->prompt = ft_strjoin(prompt_tmp, prompt_tmp2);
	free (prompt[0]);
	free (prompt[1]);
	free (prompt[2]);
	free (prompt[3]);
	free (code[0]);
	free (code[1]);
	free (prompt_tmp);
	free (prompt_tmp2);
	free (cfg_str_pf);
	free (cfg_str_cl);
	free (cfg_str);
	data->exit_status = 0;
	return (true);
}

bool	builtin_history(t_data *data)
{
	char	tmp_cl[9];
	char	tmp_print;
	int		fd;
	bool	flag_hist;
	bool	flag_print;
	
	if (data->argv[1] && !ft_strncmp(data->argv[1], "-c", 2))
	{
		usleep(1000);
		fd = open(CFG, O_RDONLY);
		read(fd, tmp_cl, 8);
		tmp_cl[8] = '\0';
		close (fd);
		fd = open(CFG, O_RDWR | O_TRUNC);
		write(fd, tmp_cl, 8);
		write(fd, "\n", 1);
		close (fd);
	}
	else
	{
		flag_hist = false;
		flag_print = false;
		fd = open(CFG, O_RDONLY);
		while (read(fd, &tmp_print, 1))
		{
			if (tmp_print == '\n' && !flag_hist)
				flag_hist = true;
			if (tmp_print == '=' && flag_hist)
			{
				read(fd, &tmp_print, 1);
				flag_print = true;
			}
			if (flag_print)
				write(1, &tmp_print, 1);
			if (tmp_print == '\n' && flag_hist)
				flag_print = false;
		}
		close (fd);
	}
	return (true);
}