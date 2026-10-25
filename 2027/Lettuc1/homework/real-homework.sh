#!/bin/bash

{
echo "当前时间"
date
echo -e "\n最近使用的十条命令"
tail -n 10 ~/.bash_history
echo -e "当前工作路径"
pwd 
} > "$(dirname "$0")/output.txt"
