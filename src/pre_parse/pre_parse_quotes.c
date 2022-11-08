/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:20:39 by tdehne            #+#    #+#             */
/*   Updated: 2022/11/08 17:20:50 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	new_len(size_t len_s, size_t len_n, char *needle, char *str)
{
	bool	dquote;
	bool	squote;
	size_t	i;

	i = 0;
	dquote = false;
	squote = false;
	while (ft_strlen(str + i) > len_n)
	{
		if (!dquote && !squote && !ft_strncmp(str + i, needle, len_n))
			len_s -= len_n;
		if (str[i] == '\"')
			dquote = !dquote;
		else if (str[i] == '\'')
			squote = !squote;
		i++;
	}
	return (len_s);
}

static void	check_for_quotes(bool *dquote, bool *squote, char c)
{
	if (c == '\"')
		(*dquote) = !(*dquote);
	else if (c == '\'')
		(*squote) = !(*squote);
}

static char	*realloc_str(size_t len_s, size_t len_n, char *needle, char *str)
{
	char	*ret;
	bool	dquote;
	bool	squote;
	size_t	i;
	size_t	j;

	dquote = false;
	squote = false;
	ret = ft_calloc(len_s + 1, sizeof(char));
	i = 0;
	j = 0;
	while (j < len_s)
	{
		if (!dquote && !squote && !ft_strncmp(str + i, needle, len_n))
		{
			i += len_n;
			continue ;
		}
		check_for_quotes(&dquote, &squote, str[i]);
		ret[j++] = str[i];
		if (!str[i] && !str[i + 1])
			break ;
		i++;
	}
	return (ret[j] = '\0', ret);
}

char	*strrmstr(char *str, char *needle)
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
	len_str = new_len(len_str, len_needle, needle, str);
	if (len_str == ft_strlen(str))
		return (str);
	ret = realloc_str(len_str, len_needle, needle, str);
	free_null(1, &str);
	return (ret);
}
