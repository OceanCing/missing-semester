#!/bin/bash

echo "系统时间"
date

echo  "最近十条命令"
tail -n 10 ~/.bash_history

echo  “笔记字数”
wc -m ../notes/lesson1_notes.md | awk '{print $1}'

