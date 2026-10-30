#!/bin/bash
echo "当前时间:"
date
echo "最近使用的十条命令:"
history | tail -n 10
echo "笔记字数:"
wc -m < ../notes/notes.md