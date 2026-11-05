#!/usr/bin/env bash
# Lesson 01 homework: write system time, last 10 shell commands and note word count into output.txt

HERE="$(cd "$(dirname "$0")" && pwd)"
NOTE="$(dirname "$HERE")/notes/lesson01-linux-vim-shell-git.md"

{
  echo "===== 1. 运行脚本时的系统时间 ====="
  date '+%Y-%m-%d %H:%M:%S %Z (%A)'

  echo
  echo "===== 2. 最近使用的 10 条命令 ====="
  tail -n 10 "$HOME/.bash_history" 2>/dev/null

  echo
  echo "===== 3. 笔记的字数 ====="
  echo "笔记文件：$NOTE"
  echo "行数：$(wc -l < "$NOTE")"
  echo "字符数（含空格与标点）：$(wc -m < "$NOTE")"
  echo "非空白字符数：$(tr -d '[:space:]' < "$NOTE" | wc -m)"
} > "$HERE/output.txt"

echo "Done: $HERE/output.txt"
