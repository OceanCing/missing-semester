#!/usr/bin/env bash

set -u

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
NOTES_DIR="$SCRIPT_DIR/../notes"

RUN_TIME="$(date '+%Y-%m-%d %H:%M:%S')"

HIST_FILE=""
for f in "${HISTFILE:-}" "$HOME/.zsh_history" "$HOME/.bash_history"; do
    if [ -n "$f" ] && [ -s "$f" ]; then
        HIST_FILE="$f"
        break
    fi
done

if [ -n "$HIST_FILE" ]; then
    RECENT_CMDS="$(tail -n 10 "$HIST_FILE" | sed 's/^: [0-9]*:[0-9]*;//')"
else
    RECENT_CMDS="（没有找到命令历史文件）"
fi

NOTE_FILE="$(find "$NOTES_DIR" -type f -name '*.md' 2>/dev/null | sort | head -n 1)"

if [ -n "$NOTE_FILE" ]; then
    CHAR_COUNT="$(wc -m < "$NOTE_FILE" | tr -d ' ')"
    LINE_COUNT="$(wc -l < "$NOTE_FILE" | tr -d ' ')"
    NOTE_NAME="$(basename "$NOTE_FILE")"
else
    CHAR_COUNT=0
    LINE_COUNT=0
    NOTE_NAME="（没有找到笔记文件）"
fi

{
    echo "===== 27赛季控制视觉招新培训 · 第一课作业 ====="
    echo
    echo "运行时间：$RUN_TIME"
    echo
    echo "----- 最近使用的 10 条命令 -----"
    echo "$RECENT_CMDS"
    echo
    echo "----- 笔记统计 -----"
    echo "笔记文件：$NOTE_NAME"
    echo "字数（字符）：$CHAR_COUNT"
    echo "行数：$LINE_COUNT"
} > "$OUTPUT_FILE"

cat "$OUTPUT_FILE"
echo
echo "已写入：$OUTPUT_FILE"

