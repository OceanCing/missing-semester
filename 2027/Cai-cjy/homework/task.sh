#!/bin/bash
echo "=====当前系统时间===="
date

echo -e "\n=====最近10条命令===="
tail -n 10 ~/.zsh_history

echo -e "\n====笔记文件字数===="
wc -w ../notes/linux_git_notes.md
