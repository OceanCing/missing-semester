#!/bin/bash

{
  echo "系统时间"
  date

  echo "最近使用的 10 条命令"
  tail -n 10 ~/.bash_history
  echo "笔记字数"
  wc -m ../notes/linux-shell-vim-git.md
} > output.txt
