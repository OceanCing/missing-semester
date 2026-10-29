#!/bin/bash
# homework of lesson1
# 系统时间
{
echo "System Current Time:"
date

# 最近使用的10条命令(非读取历史记录文件)
echo -e "last 10 commands:"
history | tail -n 10

# 笔记的字数
echo -e "num of words of note:"
wc -w ../notes/lesson1_note.md
}>output.txt
