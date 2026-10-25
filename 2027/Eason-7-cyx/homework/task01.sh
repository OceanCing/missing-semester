#!/bin/bash
set -o history
echo "1. 系统的时间是："
date

echo "2. 最近使用的十条命令："
history | tail -n 10

echo "3. 笔记的字数："
wc -m ~/missing-semester/2027/Eason-7-cyx/notes/linux_notes.md
