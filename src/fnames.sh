#!/bin/bash

grep -P '^void\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'
grep -P '^bool\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'
grep -P '^char\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'
grep -P '^int\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'
grep -P '^long long\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'
grep -P '^size_t\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'
grep -P '^t_data\t' $(find . -type f -name '*.c') | sort | sed 's/\.\//\/\//g' | sed 's/\/\/libft.*$//g' | sed 's/:/\n/g'