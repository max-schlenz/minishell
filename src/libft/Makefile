# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 12:57:52 by mschlenz          #+#    #+#              #
#    Updated: 2022/10/25 06:55:31 by mschlenz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL 			=	/bin/bash
UNAME			=	$(shell uname)
MAKEFLAGS 		=	--no-print-directory
CFLAGS			=	-g #-Wall -Wextra -Werror #-fsanitize=address 

#FORMAT----------------------------------#
DEFCL			=	$(shell echo -e "\033[0m")
RED				=	$(shell echo -e "\033[0;31m")
GREEN			=	$(shell echo -e "\033[0;32m")
BGREEN			=	$(shell echo -e "\033[1;32m")
YELLOW			=	$(shell echo -e "\033[0;33m")
BLUE			=	$(shell echo -e "\033[0;34m")
BBLUE			=	$(shell echo -e "\033[1;34m")
PURPLE			=	$(shell echo -e "\033[0;35m")
CYAN			=	$(shell echo -e "\033[0;36m")
BCYAN			=	$(shell echo -e "\033[1;36m")
GRAY			=	$(shell echo -e "\033[0m\033[38;5;239m")
# ---------------------------------------#

PRINT_OBJ_DIR = $(shell echo "$$PWD/$(OBJ_DIR)" | sed -e 's/^.*src/src/g')

NAME 		= 	libft

INC_DIR		=	../../inc/
SRC_DIR		=	src/
OBJ_DIR		=	obj/
LIB_DIR		=	../../lib/

SRC			= 	ft_isalnum ft_isalpha ft_isascii ft_isdigit ft_isprint															\
				ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd															\
				ft_toupper ft_tolower ft_itoa ft_atoi ft_strlen ft_strchr ft_strrchr ft_strncmp									\
				ft_strlcat ft_strlcpy ft_strdup ft_strnstr ft_strjoin ft_substr ft_striteri ft_strtrim ft_split ft_strmapi		\
				ft_calloc ft_memchr ft_memcmp ft_memcpy ft_memmove ft_memset ft_bzero											\
				ft_lstnew ft_lstadd_back ft_lstadd_front ft_lstclear ft_lstdelone ft_lstiter ft_lstlast ft_lstsize				\
				get_next_line/get_next_line get_next_line/get_next_line_utils													\
				ft_printf/ft_printf ft_printf/ft_printf_csi ft_printf/ft_printf_p ft_printf/ft_printf_u ft_printf/ft_printf_xX	\

SRC_PATH	=	$(addsuffix .c, $(addprefix $(SRC_DIR), $(SRC)))
OBJ_PATH	=	$(addsuffix .o, $(addprefix $(OBJ_DIR), $(SRC)))

FLAGS		=	-Wall -Wextra -Werror
INCLUDES	=	-I $(INC_DIR)

all: create_dirs $(OBJ_PATH) $(NAME)

obj/%.o : src/%.c 
	@echo -en "\\r		➜  $(BCYAN)libft${DEFCL}...        »  $@\033[K"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ_PATH)
	@ar -rc $(LIB_DIR)$(NAME).a $(OBJ_PATH)
	@echo -e "\\r${DEFCL}		  libft\033[K   	   ✔  ${GRAY}lib/$(NAME)${DEFCL}\t\t\033[K"

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/get_next_line
	@mkdir -p $(OBJ_DIR)/ft_printf
	@mkdir -p $(LIB_DIR)
	
clean:
	@if [[ -n `find $(OBJ_DIR) -type f -name '*.o'` ]] > /dev/null 2>&1; then \
		echo -en "\\r		➜ ${LBLUE}libft${DEFCL}..."; \
		find $(OBJ_DIR) -type f -name '*.o' -delete > /dev/null 2>&1; \
		echo -e "\\r${DEFCL}		  libft\t\t   🗑  ${RED}${PRINT_OBJ_DIR}${DEFCL}\033[K"; \
	fi

fclean: clean
	@if [ -f "$(LIB_DIR)libft.a" ]; then \
		rm -f $(LIB_DIR)libft.a; \
		echo -e "\\r		  libft   	   🗑  ${RED}lib/$(NAME)${DEFCL}\033[K"; 	\
	fi
	
re: fclean $(NAME)

.PHONY: clean fclean re bonus