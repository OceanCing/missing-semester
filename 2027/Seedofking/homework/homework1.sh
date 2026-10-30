#!/bin/bash
echo "当前系统时间:" > output.txt
date >> output.txt
echo "最近的10条命令:" >> output.txt
#非交互式终端无法直接使用history 10
#开启历史功能
set -o history
#指定历史文件
HISTFILE=~/.bash_history
#从文件读取历史到内存
history -r
history 10 >> output.txt
echo "字符数为:" >> output.txt
wc -c /home/seed/DX/missing-semester/2027/Seedofking/notes/Lesson_1_Notes.md >> output.txt
echo "homework1.sh已执行, 输出已重定向到output.txt"
