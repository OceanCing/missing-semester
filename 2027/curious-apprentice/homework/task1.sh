#!/bin/bash
# 1.打印当前系统时间
echo "系统时间:"
date

# 2.打印最近使用的10条命令
echo "10条命令:"
tail -n 10 ~/.bash_history

# 3.统计笔记字数
echo "字数统计:"
wc -m ../notes/lesson1.md

