#!/bin/bash
# 1.输出当前时间
echo "===当前时间===" > output.txt
date >> output.txt
# 2.输出最近十条命令
echo -e "\n===最近十条命令===" >> output.txt
history | tail -n 10 >> output.txt
# 3.输出笔记字数
echo -e "\n===笔记字数===" >> output.txt
wc -m ../notes/lesson1.md >> output.txt
