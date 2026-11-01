#!/bin/bash

>output.txt

echo "系统时间是：">> output.txt
date >> output.txt
echo "最近使用的十条命令：">> output.txt
tail /root/.bash_history >> output.txt
echo "笔记的字数：" >> output.txt
wc -m < ../notes/First_note.md >> output.txt

