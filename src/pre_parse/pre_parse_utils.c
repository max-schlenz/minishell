/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:57:17 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/08 16:35:32 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pre_parse_check_ops(t_data *data, char **cmd, char op, int i)
{
	if ((*cmd) && (*cmd)[i] && (*cmd)[i + 1])
	{
		if ((*cmd)[i] != ' ' && (*cmd)[i] != op && (*cmd)[i + 1] == op)
			(*cmd) = pre_parse_insert_space(data, (*cmd), i);
		else if ((*cmd)[i + 1] != ' ' && (*cmd)[i + 1] != op && (*cmd)[i] == op)
			(*cmd) = pre_parse_insert_space(data, (*cmd), i);
	}
}

static size_t	new_str_len(size_t len_str, size_t len_needle, char *needle, char *str)
{
	bool	dquote;
	bool	squote;
	size_t	i;

	i = 0;
	dquote = false;
	squote = false;
	while (ft_strlen(str + i) > len_needle)
	{
		if (!dquote && !squote && !ft_strncmp(str + i, needle, len_needle))
			len_str -= len_needle;
		if (str[i] == '\"')
			dquote = !dquote;
		else if (str[i] == '\'')
			squote = !squote;
		i++;
	}
	return (len_str);
}

static char *realloc_str(size_t len_str, size_t len_needle, char *needle, char *str)
{
	char	*ret;
	bool	dquote;
	bool	squote;
	size_t	i;
	size_t	j;

	dquote = false;
	squote = false;
	ret = ft_calloc(len_str + 1, sizeof(char));
	i = 0;
	j = 0;
	while (j < len_str)
	{
		if (!dquote && !squote && !ft_strncmp(str + i, needle, len_needle))
		{
			i += len_needle;
			continue ;
		}
		if (str[i] == '\"')
			dquote = !dquote;
		else if (str[i] == '\'')
			squote = !squote;
		ret[j] = str[i];
		j++;
		if (str[i] && str[i + 1])
			i++;
		else
			break ;
	}
	ret[j] = '\0';
	return (ret);
}

char *strrmstr(char *str, char *needle)
{
	size_t		len_str;
	size_t		len_needle;
	char		*ret;

	if (!*str || !*needle)
		return (str);
	len_str = ft_strlen(str);
	len_needle = ft_strlen(needle);
	if (len_str == len_needle)
	{
		if (!ft_strncmp(str, needle, len_needle))
		{
			free_null(1, &str);
			return (ft_strdup(""));
		}
		else
			return (str);
	}
	len_str = new_str_len(len_str, len_needle, needle, str);
	if (len_str == ft_strlen(str))
		return (str);
	ret = realloc_str(len_str, len_needle, needle, str);
	free_null(1, &str);
	return (ret);
}
