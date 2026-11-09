#!/bin/bash
set -o history

echo  "运行脚本时的系统时间："
date
echo ""

echo "最近使用的10条命令："
history|tail -n 10
echo""

echo "笔记的字数："
wc -w "../notes/dxnotes.md"

