#!/bin/bash
#定义输出文件
OUTPUT_FILE="output.txt"

#1.输出运行时的系统时间
echo "======运行时间=======">"$OUTPUT_FILE"
date >> "$OUTPUT_FILE"

#2.输出最近使用的10条命令
#读取历史记录文件
echo "======最近使用的十条命令======">>"$OUTPUT_FILE"
#读取（~/。bash_history)的最后十行并加入OUTPUT_FILE
tail -n 10 ~/.bash_history >> "$OUTPUT_FILE"
#3.输出笔记的字数
echo "=====笔记的字数=====" >> "$OUTPUT_FILE"
wc -m ../notes/rm_note1.md | awk '{print $1}' >> "$OUTPUT_FILE"
#不想要显示路径，询问ai得知可以用awk
