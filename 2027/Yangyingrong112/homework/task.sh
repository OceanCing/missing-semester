#/bin/bash
#1.获取系统时间
echo "当前系统时间$(date '+%Y-%m-%d %H:%M:%S')"
#2.获取最近使用的十条命令
echo "最近使用的十条命令:"
history | tail -n 10
#3.计算笔记字数
word_count=$(wc -w < ../notes/lecture-1.md)
echo "笔记字数:$word_count"

