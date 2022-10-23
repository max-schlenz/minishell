/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 11:01:47 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/23 11:37:37 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	enum_files(char *dir)
{
	int				num;
	DIR				*dirp;

	num = 1;
	dirp = opendir(dir);
	while (readdir(dirp))
		num++;
	closedir(dirp);
	return (num);
}
