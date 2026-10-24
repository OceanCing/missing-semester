#!/bin/bash
{
  echo "系统时间"
  date
  echo "最近使用的十条命令"
  tail -n 10 ~/.bash_history
  echo "笔记字数"
  wc -w ../notes/README.md|awk'{print$1}'
} >output.txt
