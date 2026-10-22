#!/bin/bash

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# 定义文件路径
NOTES="$SCRIPT_DIR/../notes/linux_notes.md"
OUTPUT="$SCRIPT_DIR/output.txt"

# 将运行结果写入 output.txt
{
    echo "===== System Time ====="
    date '+%Y-%m-%d %H:%M:%S'

    echo ""
    echo "===== Last 10 Commands ====="
    if [ -f "$HOME/.bash_history" ]; then
        tail -n 10 "$HOME/.bash_history"
    else
        echo "No history found"
    fi

    echo ""
    echo "===== Notes Character Count ====="
    wc -m < "$NOTES"

} > "$OUTPUT"

# 显示结果
cat "$OUTPUT"
