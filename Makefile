# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 12:57:52 by mschlenz          #+#    #+#              #
#    Updated: 2022/09/20 15:41:36 by mschlenz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = /bin/bash

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

MAKEFLAGS 		=	--no-print-directory
FLAGS			= 	-g #-fsanitize=address #-Wall -Wextra -Werror

NAME			=	minishell

SRC_DIR			=	src
OBJ_DIR			=	obj
LIB_DIR			=	lib
INC_DIR			=	inc

SRC				= 	${NAME}						\
					parse/envp					\
					parse/utils					\
					exec/builtins				\
					exec/exec					\
					utils/init					\
					utils/cleanup				\
					utils/signal				\
					utils/utils					\
					utils/config				\
					parse/argv					\
					parse/syntax				\
					parse/pre_parse				\
					parse/wildcards
INC				=	libft						\
					${NAME}

LIB				=	libft 

SRC_FILES		=	$(addsuffix .c, $(addprefix $(SRC_DIR)/, $(SRC)))
LIB_FILES		=	$(addsuffix .a, $(addprefix $(LIB_DIR)/, $(LIB)))
OBJ_FILES		=	$(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(SRC)))
INC_FILES		=	$(addsuffix .h, $(addprefix $(INC_DIR)/, $(INC)))

INCLUDES		= 	-I ${INC_DIR} -I ~/.brew/opt/readline/include
LINKER			=	-Llib -lft -lreadline -L ~/.brew/opt/readline/lib
BREW			=	/Users/${USER}/.brewconfig.zsh
BREWFL			=	.brew

all: $(NAME)

$(LIB_FILES): header
	@touch .libft
	@echo -n "compiling:"
	@make -C src/libft

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/exec
	@mkdir -p $(OBJ_DIR)/parse
	@mkdir -p $(OBJ_DIR)/utils

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@gcc $(FLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIB_FILES) $(OBJ_DIR) $(OBJ_FILES) $(INC_FILES) header_c message
	@gcc $(FLAGS) -o $(NAME) $(OBJ_FILES) $(INCLUDES) $(LINKER)
	@echo -e "\\r		  ${BGREEN}$(NAME)${DEFCL}   	   ✅\n"
	
$(BREWFL):
	@if [ ! -f $(BREW) ]; then \
		echo "$(PURPLE)Installing brew & readline...$(DEFCL)";					\
		curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh;	\
		brew install readline;													\
	fi;
	@touch .brew

message:
	@if	[ ! -f ".libft" ]; then													\
		echo -en "\ncompiling:";												\
	fi
	@rm -f .libft
	@echo -en "\\r		➜ ${CYAN}$(NAME)${DEFCL}..."

clean: header
	@echo
	@rm -f .header
	@make clean -C src/libft
	@if find $(OBJ_DIR) -type f -name '*.o' -delete > /dev/null 2>&1; then		\
		echo -e "\\r		  $(NAME)   	   ✅"; 		                       	\
	fi
	@echo																			
	@if [ -f ".brew" ]; then 													\
		rm -f .brew;															\
	fi

fclean: clean header
	@if [ -f "${NAME}" ]; then													\
 		rm -f ${NAME};															\
	fi
	@make fclean -C src/libft

header:
	@if [ ! -f ".header" ]; then												\
		echo 	"$(BLUE) __  __ _       _     _          _ _ ";					\
		echo 	"|  \/  (_)_ __ (_)___| |__   ___| | $(CYAN)|";					\
		echo 	"$(BLUE)| |\/| | | '_ \| / __| '_ \ / $(CYAN)_ \ | |";			\
		echo 	"$(BLUE)| |  | | | | | | \$(CYAN)__ \ | | |  __/ | |";			\
		echo 	"$(BLUE)|_|  |_|$(CYAN)_|_| |_|_|___/_| |_|\___|_|_|";			\
		echo	"$(BCYAN)	    	       by Talea & Max$(DEFCL)";				\
		touch .header;															\
	fi

header_c:
	@rm -f .header

re: fclean all

.INTERMEDIATE: message header header_c

.PHONY: all clean fclean re bonus