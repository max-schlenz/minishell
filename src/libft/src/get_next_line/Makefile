# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 12:57:52 by mschlenz          #+#    #+#              #
#    Updated: 2022/08/30 15:12:16 by mschlenz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# #FORMAT----------------------------------#
# DEFCL	=	$(shell echo -e "\033[0m")
# RED		=	$(shell echo -e "\033[0;31m")
# GREEN	=	$(shell echo -e "\033[0;32m")
# BGREEN	=	$(shell echo -e "\033[1;32m")
# LBLUE	=	$(shell echo -e "\033[0;34m")
# LCYAN	=	$(shell echo -e "\033[0;36m")
# PURPLE	=	$(shell echo -e "\033[0;35m")
# # ---------------------------------------#

#FORMAT----------------------------------#
DEFCL	= \033[0m
RED		= \033[0;31m
GREEN	= \033[0;32m
BGREEN	= \033[1;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
BBLUE	= \033[1;34m
PURPLE	= \033[0;35m
CYAN	= \033[0;36m
BCYAN	= \033[1;36m
# ---------------------------------------#

INC_DIR		=	../../inc/
SRC_DIR		=	src/
OBJ_DIR		=	obj/
LIB_DIR		=	../../../../lib/

SRC			= 	get_next_line \
				get_next_line_utils

SRC_PATH	=	$(addsuffix .c, $(addprefix $(SRC_DIR), $(SRC)))
OBJ_PATH	=	$(addsuffix .o, $(addprefix $(OBJ_DIR), $(SRC)))

FLAGS		=	-Wall -Wextra -Werror
INCLUDES	= 	-I $(INC_DIR)

LIBFT		=	$(LIB_DIR)libft.a

all: create_dirs $(OBJ_PATH)
	@echo -e "\\r		  ${DEFCL}get_next_line    ✅ "

obj/%.o : src/%.c message
	@gcc $(FLAGS) $(INCLUDES) -c $< -o $@
	@ar -rc $(LIBFT) $@

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)


.INTERMEDIATE: message

message:
	@echo -n "		➜ ${LBLUE}get_next_line${DEFCL}..."

clean:
	@if [[ -n `find $(OBJ_DIR) -type f -name '*.o'` ]]; then \
		echo -n "		get_next_line..."; \
		find $(OBJ_DIR) -type f -name '*.o' -delete; \
		echo -e "\\r${DEFCL}		  get_next_line    ✅ "; \
	fi

fclean: clean

re: fclean all

.PHONY: all clean fclean re