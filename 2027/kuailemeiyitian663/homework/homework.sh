#!/usr/bin/env bash
# 第一课作业脚本：输出系统时间、最近10条命令、笔记字数，重定向到同目录 output.txt

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTES_FILE="$SCRIPT_DIR/../notes/lesson1-notes.md"
OUT_FILE="$SCRIPT_DIR/output.txt"

{
  echo "===== 1. 系统时间 ====="
  date

  echo
  echo "===== 2. 最近使用的 10 条命令 ====="
  HIST="${HISTFILE:-$HOME/.bash_history}"
  if [ -f "$HIST" ]; then
    tail -n 10 "$HIST"
  else
    echo "未找到历史文件：$HIST"
  fi

  echo
  echo "===== 3. 笔记字数 ====="
  if [ -f "$NOTES_FILE" ]; then
    COUNT=$(wc -m < "$NOTES_FILE" | tr -d ' ')
    echo "lesson1-notes.md 字符数（含空格/标点）：$COUNT"
  else
    echo "未找到笔记文件：$NOTES_FILE"
  fi

  echo
  echo "（本文件由脚本生成于：$SCRIPT_DIR）"
} > "$OUT_FILE"

echo "已生成：$OUT_FILE"
