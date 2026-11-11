#!/bin/bash
SCRIPT_DIR=$(dirname "$0")
NOTE_FILE="$SCRIPT_DIR/../notes/first_lesson_note.md"
echo "系统时间："
date '+%Y-%m-%d %H:%M:%S'
echo "最近使用的10条命令："
tail -n 10 "$HOME/.bash_history"
echo "笔记字符数："
wc -m < "$NOTE_FILE"
