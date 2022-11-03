#!/bin/bash

echo -n "/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 10:56:28 by mschlenz          #+#    #+#             */
/*   Updated: 2022/11/03 10:56:28 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef $(basename ${PWD^^}_H)
# define $(basename ${PWD^^}_H)
"

grep -P '^void|^bool|^char|^int|^long|^size_t|^t_data\t' $(find . -type f -name '*.c')	\
| sort																					\
| sed 's/\.\//\/\//g'																	\
| sed 's/\/\/src\/libft.*$//g'															\
| sed 's/:/\n/g'																		\
| awk '!x[$0]++'																		\
| sed s/^\\//\\n\\//g																	\
| sed s/\\t/\\t\\t\\t/g																	\
| sed "s/^int\t/int\t\t/g"																\
| sed 's/long long\t/long long/g'														\
| sed '/^$/N;/^\n$/D'

echo -e "\n#endif"