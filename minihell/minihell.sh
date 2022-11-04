#!/bin/bash

DEFCL='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
BGREEN='\033[1;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
BBLUE='\033[1;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
BCYAN='\033[1;36m'
GRAY='\033[0m\033[38;5;239m'

make -C ../
cp ../minishell ./
rm diffs.txt 2>/dev/null
while read -r line;
	do
		BASH_STDOUT=$((bash -c "$line" | cat -e) 2>/dev/null);
		BASH_EXIT=$?;
		MS_STDOUT=$((./minishell -c "$line" | cat -e) 2>/dev/null);
		MS_EXIT=$?;
		if [ "$BASH_EXIT" == "$MS_EXIT" ]; then
			EXIT_OK=true
			CL_EXIT=${GREEN}
		else
			EXIT_OK=false
			CL_EXIT=${RED}
		fi
		if [ "$BASH_STDOUT" == "$MS_STDOUT" ]; then
			OUT_OK=true
			CL_STDOUT=${GREEN}
		else
			OUT_OK=false
			CL_STDOUT=${RED}
		fi
		if [ "$EXIT_OK" == false ] || [ "$OUT_OK" == false ]; then
			echo -e "------------------"
			echo -e "\\r\033[2K${RED}$line${DEFCL}"
			echo -e "|${GREEN}$BASH_EXIT${DEFCL}| bash: \n ${GREEN}$BASH_STDOUT${DEFCL}"
			echo -e "|${CL_EXIT}$MS_EXIT${DEFCL}| minishell:\n ${CL_STDOUT}$MS_STDOUT${DEFCL}"
			echo $line >> diffs.txt
			echo -e "------------------"
		else
			echo -e "\\r\033[2K${GREEN}$line${DEFCL}"
		fi
		rm out* 2>/dev/null
done < tests/tests.txt