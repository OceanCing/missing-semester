#!/bin/bash

echo "=== 当前系统时间 ==="
date

echo -e "\n=== 最近使用的10条命令 ==="
cat ~/.bash_history | tail -n 10

echo -e "\n=== 笔记字数 ==="
wc -m ../notes/lzhbiji.md
