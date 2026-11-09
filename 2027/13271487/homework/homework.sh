#!/bin/bash
echo "当前时间" > output.txt
date >> output.txt
echo "" >> output.txt
echo "最近十条命令" >> output.txt
cat ~/.bash_history | tail -n 10 >> output.txt
echo "" >> output.txt
echo "笔记字符总数" >> output.txt
wc -c linux-note.md >> output.txt
