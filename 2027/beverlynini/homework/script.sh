#!/bin/bash
echo "==== 系统时间 ====" > output.txt
date >> output.txt
echo -e "\n==== 最近10条命令 ====" >> output.txt
tail -n 10 ~/.bash_history >> output.txt
echo -e "\n==== 笔记字数 ====" >> output.txt
wc -w ../note/10.17note.md >> output.txt
