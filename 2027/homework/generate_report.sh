#!/usr/bin/env bash

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
NOTE_FILE="$SCRIPT_DIR/../notes/first-note.md"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

show_recent_commands() {
    local history_file="${HISTFILE:-$HOME/.bash_history}"

    if [ -r "$history_file" ]; then
        tail -n 10 "$history_file"
    else
        history 10 2>/dev/null || echo "无法读取命令历史"
    fi
}

if [ -r "$NOTE_FILE" ]; then
    note_char_count="$(wc -m < "$NOTE_FILE" | tr -d '[:space:]')"
else
    note_char_count="无法读取（文件不存在：$NOTE_FILE）"
fi

{
    echo "运行脚本时的系统时间：$(date '+%Y-%m-%d %H:%M:%S %Z')"
    echo
    echo "最近使用的 10 条命令："
    show_recent_commands
    echo
    echo "笔记字数：$note_char_count"
} > "$OUTPUT_FILE"
