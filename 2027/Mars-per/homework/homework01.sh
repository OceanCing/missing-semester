#!/usr/bin/env bash
cd "$(dirname "$0")"

set -o history
history -c
history -r "${HISTFILE:-$HOME/.bash_history}" 2>/dev/null || true

{
  echo "系统时间"
  date
  echo

  echo "最近 10 条命令"
  history | tail -n 10
  echo

  echo "笔记字数"
  wc -m ../notes/lesson01.md
} > output.txt
