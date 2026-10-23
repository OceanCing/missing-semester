#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTE_FILE="$SCRIPT_DIR/../notes/lesson1-linux-vim-shell-git.md"
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

{
  echo "运行时间：$(date '+%Y-%m-%d %H:%M:%S %Z')"
  echo

  echo "最近使用的10条命令："
  if [[ -f "$HISTORY_FILE" ]]; then
    tail -n 10 "$HISTORY_FILE"
  else
    echo "未找到历史命令文件：$HISTORY_FILE"
  fi
  echo

  echo "笔记单词数（wc -w）：$(wc -w < "$NOTE_FILE")"
  echo "笔记字符数（wc -m）：$(wc -m < "$NOTE_FILE")"
} > "$SCRIPT_DIR/output.txt"
