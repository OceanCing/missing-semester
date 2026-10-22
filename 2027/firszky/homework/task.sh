#!/bin/bash
echo "=== 系统时间 ==="
date
echo -e "\n=== 最近十条命令 ==="
history 10
echo -e "\n=== 笔记字数 ==="
wc -w ../notes/firstlesson.md
