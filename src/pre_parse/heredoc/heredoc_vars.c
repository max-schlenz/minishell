/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:07:56 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/26 16:39:15 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*heredoc_get_var_name(char *line)
{
	int		i;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	return (ft_substr(line, 0, i - 1));
}

static char	*heredoc_expand_var(t_data *data, char *line, int start)
{
	char	*var_name;
	char	*var_content;

	var_name = heredoc_get_var_name(line + start);
	var_content = expand_get_var_content(data, var_name);
	free_str(1, var_name);
	return (var_content);
}

static bool	heredoc_do_expand(t_data *data, char **line, int *i)
{
	char	*str_before;
	char	*var;
	char	*str_after;

	str_before = ft_substr((*line), 0, (*i));
	var = heredoc_expand_var(data, (*line), (*i));
	while ((*line)[(*i)] && (*line)[(*i)] != ' ')
		(*i)++;
	str_after = ft_strdup((*line) + (*i));
	(*line) = str_realloc((*line), merge_str(3, str_before, var, str_after), 1);
	(*i) = 0;
	return (true);
}

char	*heredoc_vars(t_data *data, char *line)
{
	int		i;
	bool	expanded;

	i = 0;
	expanded = false;
	while (line && line[i])
	{
		if (line[i] == '$')
			expanded = heredoc_do_expand(data, &line, &i);
		if (!line[i])
			break ;
		i++;
	}
	if (!line[i] && expanded)
		line = str_realloc(line, ft_strjoin(line, "\n"), 1);
	return (line);
}
