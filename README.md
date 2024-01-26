# Minishell Project

## Introduction
Minishell is a comprehensive shell program, part of the 42 curriculum, designed to replicate the functionalities of traditional Unix shells, while using the C programming language.

## Features
- **Command Prompt**: Interactive command line interface with a working history.
- **Executable Commands**: Ability to search and launch executables (PATH, relative, absolute paths).
- **Redirections and Pipes**: Supports `<`, `>`, `<<`, `>>` redirections, and `|` pipes.
- **Environment Variables**: Handles environment variables (`$VAR`, `export`, `unset`).
- **Signal Handling**: Manages signals like `CTRL-C`, `CTRL-D`, `CTRL-\`.
- **Built-in Commands**: Includes `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- **Quoting Mechanism**: Correctly handles single (`'`) and double (`"`) quotes.
- **Command Separation**: Supports semicolon (`;`) for command separation.
- **Wildcard Expansion**: Implements `*` wildcard expansion.
- **Exit Status**: Correctly returns the exit status of commands.
- **Bonus Features**: Advanced features like heredoc (`<<`), file descriptor aggregation, and additional built-ins.

## Installation and Usage
1. Clone the repository
2. Compile the program: `make`
3. Run the shell: `./minishell`

## Technologies Used
- C programming language
- Unix system calls and libraries
