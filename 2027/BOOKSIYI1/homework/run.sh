#!/bin/bash

echo "当前系统时间: $(date)"

echo -e "\n最近使用的10条命令:"
history | tail -n 10

echo -e "\n笔记的字数:"
wc -m "2027/BOOKSIYI1/notes/第一课笔记.md"
