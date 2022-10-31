# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/22 12:57:52 by mschlenz          #+#    #+#              #
#    Updated: 2022/10/31 11:46:04 by mschlenz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL 			=	/bin/bash
UNAME			=	$(shell uname)
MAKEFLAGS 		=	--no-print-directory
CFLAGS			=	-g #-Wall -Wextra -Werror -g #-fsanitize=address 

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

NAME			=	minishell

SRC_DIR			=	src
OBJ_DIR			=	obj
LIB_DIR			=	lib
INC_DIR			=	inc

SRC				= 	${NAME}													\
					debug													\
					exec/builtins/cd/cd										\
					exec/builtins/cd/cd_cleanup								\
					exec/builtins/cd/cd_utils								\
					exec/builtins/echo/echo									\
					exec/builtins/env/env									\
					exec/builtins/exit/exit									\
					exec/builtins/export/export								\
					exec/builtins/export/export_subshell					\
					exec/builtins/export/export_utils						\
					exec/builtins/pwd/pwd									\
					exec/builtins/unset/unset								\
					exec/builtins/where/where								\
					exec/exec												\
					exec/builtins/builtin_error								\
					exec/exec_pipes											\
					exec/exec_utils											\
					parse/expand/expand_check_var							\
					parse/expand/expand_get_var								\
					parse/expand/expand_special_cases						\
					parse/expand/expand_utils								\
					parse/expand/expand_vars								\
					parse/parse_envp										\
					parse/parse_utils										\
					parse/setup/setup										\
					parse/setup/setup_modifiers								\
					parse/setup/setup_ops									\
					parse/setup/setup_quote_escape							\
					parse/setup/setup_redir									\
					parse/setup/setup_subshell								\
					parse/setup/setup_utils									\
					parse/wildcards/wild_cases								\
					parse/wildcards/wild_enum_files							\
					parse/wildcards/wild_match								\
					parse/wildcards/wild_utils								\
					parse/wildcards/wildcards								\
					pre_parse/heredoc/heredoc								\
					pre_parse/heredoc/heredoc_prompt						\
					pre_parse/heredoc/heredoc_utils							\
					pre_parse/heredoc/heredoc_vars							\
					pre_parse/pre_parse										\
					pre_parse/pre_parse_utils								\
					pre_parse/syntax/syntax									\
					pre_parse/syntax/syntax_error							\
					pre_parse/syntax/syntax_utils							\
					prompt/color/color										\
					prompt/color/color_utils								\
					prompt/prompt											\
					prompt/prompt_history									\
					prompt/prompt_prio										\
					prompt/prompt_utils										\
					utils/cleanup											\
					utils/config											\
					utils/init												\
					utils/signal											\
					utils/utils												\

INC				=	${NAME}								\
					data								\
					strings								\
					libft								

LIB				=	libft 

SRC_FILES		=	$(addsuffix .c, $(addprefix $(SRC_DIR)/, $(SRC)))
LIB_FILES		=	$(addsuffix .a, $(addprefix $(LIB_DIR)/, $(LIB)))
OBJ_FILES		=	$(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(SRC)))
INC_FILES		=	$(addsuffix .h, $(addprefix $(INC_DIR)/, $(INC)))

READLINE		=	/usr/include/readline/readline.h
INCLUDES		= 	-I ${INC_DIR}
LINKER			=	-L lib -l ft -l readline

MAC_BREW		=	/Users/${USER}/.brewconfig.zsh
MAC_READLINE	=	~/.brew/opt/readline
MAC_INCLUDES	=	-I $(MAC_READLINE)/include
MAC_LINKER		=	-L $(MAC_READLINE)/lib

all: $(NAME)

$(LIB_FILES): header
	@echo -n "compiling..."
	@touch .tmp
	@$(MAKE) MAKEFLAGS+=-j8 CFLAGS+="$(CFLAGS)" -C src/libft
	@ar -rc $(LIB_FILES) $$(find ./src/libft -type f -name '*.o')

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/exec
	@mkdir -p $(OBJ_DIR)/exec/builtins
	@mkdir -p $(OBJ_DIR)/exec/builtins/cd
	@mkdir -p $(OBJ_DIR)/exec/builtins/echo
	@mkdir -p $(OBJ_DIR)/exec/builtins/env
	@mkdir -p $(OBJ_DIR)/exec/builtins/exit
	@mkdir -p $(OBJ_DIR)/exec/builtins/export
	@mkdir -p $(OBJ_DIR)/exec/builtins/pwd
	@mkdir -p $(OBJ_DIR)/exec/builtins/unset
	@mkdir -p $(OBJ_DIR)/exec/builtins/where
	@mkdir -p $(OBJ_DIR)/parse
	@mkdir -p $(OBJ_DIR)/parse/setup
	@mkdir -p $(OBJ_DIR)/parse/expand
	@mkdir -p $(OBJ_DIR)/parse/wildcards
	@mkdir -p $(OBJ_DIR)/pre_parse
	@mkdir -p $(OBJ_DIR)/pre_parse/heredoc
	@mkdir -p $(OBJ_DIR)/pre_parse/syntax
	@mkdir -p $(OBJ_DIR)/prompt
	@mkdir -p $(OBJ_DIR)/prompt/color
	@mkdir -p $(OBJ_DIR)/utils

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c header_c 
	@if [ ! -f .tmp ]; then														\
		echo -n "compiling...";												\
		touch .tmp;\
	fi
	@echo -en "\\r		➜  ${BCYAN}$(NAME)${DEFCL}...    »  $@\033[K"
	@$(CC) $(CFLAGS) $(INCLUDES) $(MAC_INCLUDES) -c $< -o $@
	
ifeq ($(UNAME), Darwin)
$(NAME): $(MAC_BREW) $(MAC_READLINE) $(LIB_FILES) $(OBJ_DIR) $(OBJ_FILES)
	@echo -en "\\r		  ${BGREEN}$(NAME)${DEFCL}        ✔  ${BGREEN}./$(NAME)${DEFCL}\033[K\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(INCLUDES) $(MAC_INCLUDES) $(LINKER) $(MAC_LINKER)
	@rm -f .tmp
else
$(NAME): $(READLINE) $(LIB_FILES) $(OBJ_DIR) $(OBJ_FILES)
	@echo -en "\\r		  ${BGREEN}$(NAME)${DEFCL}        ✔  ${BGREEN}./$(NAME)${DEFCL}\033[K\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(INCLUDES) $(LINKER)
	@rm -f .tmp
endif

$(READLINE):
	@echo "$(CYAN)Installing readline...$(DEFCL)"
	@pacman -Syu --noconfirm readline

$(MAC_BREW):
	@echo "$(CYAN)Installing brew...$(DEFCL)"
	@curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh;
	@source ~/.zshrc

$(MAC_READLINE):
	@echo "$(PURPLE)Installing readline...$(DEFCL)"
	@brew install readline

clean: header
	@rm -f .header
	@echo -en "cleaning objs...";
	@$(MAKE) clean -C src/libft
	@if find $(OBJ_DIR) -type f -name '*.o' -delete > /dev/null 2>&1; then		\
		echo -en "\\r		  $(NAME)   	   🗑  ${RED}$(OBJ_DIR)/${DEFCL}\033[K";\
	fi
	@echo -e "\n";
	@if find $(OBJ_DIR) -type d -empty -delete > /dev/null 2>&1; then			\
		:;																		\
	fi

fclean: clean header
	@echo -en "cleaning bins..."
	@$(MAKE) fclean -C src/libft
	@if find $(LIB_DIR) -type d -empty -delete > /dev/null 2>&1; then			\
		:; \
	fi
	@if [ -f "${NAME}" ]; then													\
 		rm -f ${NAME};															\
		echo -e "\\r		  $(NAME)   	   🗑  ${RED}$(NAME)${DEFCL}\033[K"; \
	fi
	@echo -en "\n";

header:
	@if [ ! -f ".header" ]; then												\
		echo 	"$(BLUE) __  __ _       _     _          _ _ ";					\
		echo 	"|  \/  (_)_ __ (_)___| |__   ___| | $(CYAN)|";					\
		echo 	"$(BLUE)| |\/| | | '_ \| / __| '_ \ / $(CYAN)_ \ | |";			\
		echo 	"$(BLUE)| |  | | | | | | \$(CYAN)__ \ | | |  __/ | |";			\
		echo 	"$(BLUE)|_|  |_|$(CYAN)_|_| |_|_|___/_| |_|\___|_|_|";			\
		echo	"$(BCYAN)	    	       by Talea & Max$(DEFCL)";				\
		echo	"";																\
		touch .header;															\
	fi

header_c:
	@rm -f .header

test:
	@cd tests && bash tester.sh a

re: fclean all

.INTERMEDIATE: header header_c

.PHONY: all clean fclean re bonus