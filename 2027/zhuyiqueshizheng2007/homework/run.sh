#!/bin/bash

HISTFILE=~/.bash_history

# 1. 输出系统时间 (查阅 date 命令，用重定向 > 写入 output.txt)
echo "系统时间：" > output.txt
date >> output.txt

# 2. 最近使用的10条命令 (用 history 结合管道 | 和 tail，追加 >> 到 output.txt)
echo "最近10条命令：" >> output.txt
cat ~/.bash_history | tail -n 10 >> output.txt

# 3. 你的笔记的字数 (用 wc -w 命令统计 lesson1.md，追加 >> 到 output.txt)
echo "笔记字数：" >> output.txt
wc -w notes/*.md >> output.txt
