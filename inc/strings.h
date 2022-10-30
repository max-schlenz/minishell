/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdehne <tdehne@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:28:41 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/30 13:58:04 by tdehne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGS_H
# define STRINGS_H

# include <minishell.h>

# define E_MEM_MSG	"Failed to allocate memory."
# define E_FORK_MSG	"Failed to create Forks."
# define E_RW_MSG "Error: R/W not permitted\n"
# define E_NC_QUOTE "Error: unclosed quotes!\n"
# define E_TM_ARG "Error: too many arguments\n"
# define E_EXIT_REQNO "Error: exit: numeric argument required: "
# define E_EXPORT_CONT "Error: export: not valid in this context: "
# define E_EXPORT_IDENT "Error: export: not a valid identifier: "
# define E_EXPORT_INVOPT "Error: export: invalid option: "
# define E_ENV_IDENT "Error: env: not a valid identifier: "
# define E_ENV_INVOPT "Error: env: invalid option: "
# define E_EXECVE_MSG "Failed to call execve."
# define E_EXEC_ISDIR ": is a directory"
# define E_EXEC_NOTFOUND ": command not found"
# define E_CD_OLDPWD_MSG "Error: cd: OLDPWD not set\n"
# define E_FILE_NOT_FOUND "Error: No such file or directory "

# define PROMPT_SUFFIX "] \x01\033[0;1m\x02#\x01\033[0m\x02 "

//Colors
# define WHITE "\x01\033[0m\x02"
# define RED "\x01\033[31;1m\x02"
# define GREEN "\x01\033[32;1m\x02"
# define YELLOW "\x01\033[33;1m\x02"
# define BLUE "\x01\033[34;1m\x02"
# define MAGENTA "\x01\033[35;1m\x02"
# define CYAN "\x01\033[36;1m\x02"
# define GRAY "\x01\033[0m\x02\x01\033[38;5;239m\x02"

#endif