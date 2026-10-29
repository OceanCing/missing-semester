#!/bin/bash

echo "=== 运行时的系统时间 ===" > output.txt
date >> output.txt


echo -e "\n=== 最近 10 条命令 ===" >> output.txt
tail -n 10 ~/.bash_history >> output.txt


echo -e "\n=== 笔记的字数 ===" >> output.txt
wc -m ../notes/lecture1.md >> output.txt



