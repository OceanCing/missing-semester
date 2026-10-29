#!/bin/bash
# hw1.sh - 第一课作业

cd "$(dirname "${BASH_SOURCE[0]}")" || exit 1

{
  echo "=== 系统时间 ==="
  date

  echo
  echo "=== 最近使用的 10 条命令 ==="
  history 10

  echo
  echo "=== 笔记字数 ==="
  wc -m ../notes/lesson1-linux-vim-git.md
} > output.txt

echo "已生成 output.txt"
