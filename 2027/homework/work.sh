#!/bin/bash

time=$(date +"%Y-%m-%d %H:%M:%S")
cmd=$(tail -n 10 ~/.bash_history)
file=~/note/missing-semester/2027/notes/first.md
word=$(wc -w<"$file")

{
echo "===运行脚本的系统时间==="
echo "$time"
echo ""

echo "===最近使用的10条命令==="
echo "$cmd"
echo ""

echo "===我的笔记字数==="
echo “$word”
echo ""
}> output.txt

