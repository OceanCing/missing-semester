#!/bin/bash

# 脚本所在目录，保证 output.txt 和脚本同路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT="$SCRIPT_DIR/output.txt"

# 笔记文件路径：2027/zkz/notes/lesson01-linux-vim-shell-git.md
NOTES="$SCRIPT_DIR/../notes/lesson01-linux-vim-shell-git.md"

{
  echo "===== 系统时间 ====="
  date

  echo
  echo "===== 最近使用的 10 条命令 ====="
  # 优先读取 ~/.bash_history 的最后 10 条；没有则提示
  if [ -f "$HOME/.bash_history" ]; then
    tail -n 10 "$HOME/.bash_history"
  else
    echo "(未找到 ~/.bash_history)"
  fi

  echo
  echo "===== 笔记字数 ====="
  if [ -f "$NOTES" ]; then
    echo "字符数: $(wc -m < "$NOTES")"
    echo "单词数: $(wc -w < "$NOTES")"
    echo "行数:   $(wc -l < "$NOTES")"
  else
    echo "笔记文件不存在: $NOTES"
  fi
} > "$OUTPUT"

echo "已写入: $OUTPUT"
