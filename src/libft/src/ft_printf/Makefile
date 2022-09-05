# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 12:57:52 by mschlenz          #+#    #+#              #
#    Updated: 2022/09/05 12:07:33 by mschlenz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = /bin/bash
OS := $(shell uname -s)

#FORMAT----------------------------------#
DEFCL	=	$(shell echo -e "\033[0m")
RED		=	$(shell echo -e "\033[0;31m")
GREEN	=	$(shell echo -e "\033[0;32m")
BGREEN	=	$(shell echo -e "\033[1;32m")
YELLOW	=	$(shell echo -e "\033[0;33m")
BLUE	=	$(shell echo -e "\033[0;34m")
BBLUE	=	$(shell echo -e "\033[1;34m")
PURPLE	=	$(shell echo -e "\033[0;35m")
CYAN	=	$(shell echo -e "\033[0;36m")
BCYAN	=	$(shell echo -e "\033[1;36m")
# ---------------------------------------#

NAME 	=	libft.a

INC_DIR		=	../../../../inc/
SRC_DIR		=	src/
OBJ_DIR		=	obj/
LIB_DIR		=	../../../../lib/

SRC			= 	ft_printf \
				ft_printf_csi \
				ft_printf_p \
				ft_printf_u \
				ft_printf_xX

SRC_PATH	=	$(addsuffix .c, $(addprefix $(SRC_DIR), $(SRC)))
OBJ_PATH	=	$(addsuffix .o, $(addprefix $(OBJ_DIR), $(SRC)))

FLAGS		=	-Wall -Wextra -Werror
INCLUDES	= 	-I $(INC_DIR)

all: create_dirs $(OBJ_PATH)
	@echo -e "\\r		  ${DEFCL}ft_printf    	   ✅ "

obj/%.o : src/%.c message
	@gcc $(FLAGS) $(INCLUDES) -c $< -o $@
	@ar -rc $(LIB_DIR)$(NAME) $@

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

.INTERMEDIATE: message

message:
	@echo -n " 		➜ ${LBLUE}ft_printf${DEFCL}..."

clean:
	@if [[ -n `find $(OBJ_DIR) -type f -name '*.o'` ]]; then \
		echo -n "		  ft_printf..."; \
		find $(OBJ_DIR) -type f -name '*.o' -delete; \
		echo -e "\\r${DEFCL}		  ft_printf   	   ✅ "; \
	fi

fclean: clean
	@if [ -f "$(LIB_DIR)libftprintf.a" ]; then \
		echo "${PURPLE}removing ${LBLUE}ft_printf${PURPLE} library file...${DEFCL}"; \
		rm -f $(LIB_DIR)libftprintf.a; \
	fi

re: fclean all

.PHONY: all clean fclean re