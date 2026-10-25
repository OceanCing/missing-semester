#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
NOTES="$SCRIPT_DIR/../notes/note_1.md"
OUTPUT="$SCRIPT_DIR/output.txt"

exec > "$OUTPUT" 2>&1

echo "===== 系统时间 ====="
date "+%Y-%m-%d %H:%M:%S"

echo "===== 最近10条命令 ====="
tail -n 10 ~/.bash_history

echo "===== 笔记字数 ====="
wc -m < "$NOTES"

