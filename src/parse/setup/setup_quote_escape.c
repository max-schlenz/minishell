/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_quote_escape.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 12:22:50 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/27 17:49:23 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*rm_quotes_start(t_data *data, int *i, int i_arg, char *tmp)
{
	char	*str_before_q;
	char	*str_wo_q;
	char	*ret;
	char	delim;

	delim = data->argv[i_arg][(*i)];
	if (!tmp && (*i) > 0)
		str_before_q = ft_substr(data->argv[i_arg], 0, (*i));
	else
		str_before_q = tmp;
	while (data->argv[i_arg][(*i)] && data->argv[i_arg][(*i)] == delim)
		(*i)++;
	data->rmq.start = (*i);
	if (data->argv[i_arg][(*i)] && data->argv[i_arg][(*i) + 1])
		(*i)++;
	while (data->argv[i_arg][(*i)] && data->argv[i_arg][(*i)] != delim)
		(*i)++;
	data->rmq.end = (*i);
	if (data->rmq.start != data->rmq.end)
		str_wo_q = ft_substr(data->argv[i_arg], data->rmq.start,
				data->rmq.end - data->rmq.start);
	else
		str_wo_q = ft_strdup("");
	ret = ft_strjoin_dup(str_before_q, str_wo_q);
	return (free_str(2, str_wo_q, str_before_q), ret);
}

void	rm_quotes_wr_argv(t_data *data, int i_arg, char *tmp)
{
	char	*argv;

	if (ft_strlen(data->argv[i_arg]) >= data->rmq.end + 1)
		argv = ft_strdup(data->argv[i_arg] + data->rmq.end + 1);
	else
		argv = ft_strdup("");
	free(data->argv[i_arg]);
	data->argv[i_arg] = NULL;
	data->argv[i_arg] = ft_strjoin_dup(tmp, argv);
	free (tmp);
	free (argv);
}

char	*rm_quotes_mid(t_data *data, int *i, int i_arg, char *argv)
{
	char	*str_between;
	char	*ret;
	char	*tmp;

	str_between = ft_substr(data->argv[i_arg], (*i), 1);
	data->rmq.end++;
	tmp = argv;
	ret = ft_strjoin_dup(argv, str_between);
	free (tmp);
	free (str_between);
	return (ret);
}

void	remove_quotes(t_data *data, int i_arg)
{
	char	*argv;
	int		i;

	rm_quotes_init(data);
	argv = NULL;
	i = 0;
	while (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
	{
		if (data->argv[i_arg][i] == '\\')
			i++;
		else if (!data->rmq.start
			&& (data->argv[i_arg][i] == '\"' || data->argv[i_arg][i] == '\''))
		{
			data->rmq.f_rmq = true;
			argv = rm_quotes_start(data, &i, i_arg, argv);
		}
		else if (data->rmq.f_rmq)
			argv = rm_quotes_mid(data, &i, i_arg, argv);
		data->rmq.start = 0;
		if (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
			i++;
	}
	if (argv)
		rm_quotes_wr_argv(data, i_arg, argv);
}

void	remove_backslashes(t_data *data, int i_arg)
{
	int		i;

	i = 0;
	while (data->argv[i_arg][i])
	{
		if (data->argv[i_arg][i] == '\\' && data->argv[i_arg][i + 1] == ' ')
			data->argv[i_arg][i] = ' ';
		else if (data->argv[i_arg][i] == '\\')
		{
			while (data->argv[i_arg][i] && data->argv[i_arg][i + 1])
			{
				data->argv[i_arg][i] = data->argv[i_arg][i + 1];
				i++;
			}
			data->argv[i_arg][i] = '\0';
		}
		i++;
	}
}
