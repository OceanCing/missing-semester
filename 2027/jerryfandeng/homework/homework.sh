#!/usr/bin/env bash

# 找到脚本所在目录，以及 notes 目录
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
USER_DIR="$(dirname "$SCRIPT_DIR")"
NOTES_DIR="$USER_DIR/notes"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

# 所有报告内容重定向到 output.txt
{
    echo "===== System Time ====="
    date '+%Y-%m-%d %H:%M:%S'

    echo "===== Last 10 Commands ====="

    if [[ -r "$HISTORY_FILE" ]]; then
        set -o history
        history -r "$HISTORY_FILE"
        history 10
    else
        echo "Bash history file not found."
    fi

    echo
    echo "===== Notes Word Count ====="

    shopt -s nullglob
    notes=("$NOTES_DIR"/*.md)

    if ((${#notes[@]} > 0)); then
        word_count=$(cat -- "${notes[@]}" | wc -w)
        echo "Total words: $word_count"
    else
        echo "No Markdown files found in notes/"
    fi

} > "$OUTPUT_FILE"

echo "Results saved to: $OUTPUT_FILE"
