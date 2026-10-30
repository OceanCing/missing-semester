#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTES_DIR="$SCRIPT_DIR/../notes"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
HIST_FILE="${HISTFILE:-$HOME/.bash_history}"

{
    echo "===== 系统时间 ====="
    date
    echo

    echo "===== 最近使用的10条命令 ====="
    if [ -r "$HIST_FILE" ]; then
        tail -n 10 "$HIST_FILE"
    else
        echo "无法读取命令历史文件：$HIST_FILE"
    fi
    echo

    echo "===== 笔记字数 ====="
    if [ -d "$NOTES_DIR" ]; then
        find "$NOTES_DIR" -maxdepth 1 -type f -name '*.md' -exec cat {} + 2>/dev/null | wc -m
    else
        echo "0"
    fi
} > "$OUTPUT_FILE"

echo "结果已保存到 $OUTPUT_FILE"
