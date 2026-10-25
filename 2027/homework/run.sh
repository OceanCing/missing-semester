#!/bin/bash
>output.txt
echo "运行脚本时的系统时间" >> output.txt
date >> output.txt
echo "" >> output.txt

echo "最近使用的10条命令" >> output.txt
tail -n 10 ~/.bash_history >> output.txt
echo "" >> output.txt

echo "你的笔记的字数" >> output.txt
wc -w ../notes/DX_train_1.md >> output.txt
