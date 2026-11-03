#!/bin/bash

# 1.输出系统时间
echo "==== 当前系统时间 ===="
date

# 2.最近10条历史命令
echo -e "\n==== 最近10条命令 ===="
tail -n 10 ~/.bash_history

#3.笔记文件字数统计
NOTE_FILE="../notes/lesson1w-notes.md"
echo -e "\n==== 笔记文件字数 ===="
wc -w "$NOTE_FILE"
