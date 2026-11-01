#!/bin/bash

echo "=== 当前系统时间 ===" > output.txt
date >> output.txt

echo -e "\n=== 最近使用的10条命令 ===" >> output.txt
tail -n 10 ~/.bash_history >> output.txt

echo -e "\n=== 笔记字数统计 ===" >> output.txt
wc -m ../notes/Lesson1_note.md >> output.txt