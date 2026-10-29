#!/bin/bash

# 1. 输出当前系统时间
echo "=== 当前系统时间 ==="
date

# 2. 输出最近使用的10条命令
echo -e "\n=== 最近使用的10条命令 ==="
tail -n 10 ~/.bash_history

# 3. 输出笔记的字数统计
echo -e "\n=== 笔记字数统计 ==="
wc -m ../notes/lesson1.md
