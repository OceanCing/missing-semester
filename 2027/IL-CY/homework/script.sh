#!/bin/bash

OUTPUT="homework/output.txt"
#1.时间
echo "===系统当前时间是===" > "$OUTPUT"
date >> "$OUTPUT"

#2.获取命令
echo -e "\n===最近使用的10条命令===" >> "$OUTPUT"
tail -n 10 ~/.bash_history >> "$OUTPUT"

#3.统计字数
echo -e "\n===笔记字数统计===" >> "$OUTPUT"
wc -m notes/cul_fir.md >>"$OUTPUT"
