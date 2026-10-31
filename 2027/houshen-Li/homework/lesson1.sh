#!/usr/bin/env bash

# 设置output.txt和my_notes.md文件路径
OUTPUT_TXT="$(dirname "$0")/output.txt"
MY_NOTES_MD="$(dirname "$0")/../notes/my_notes.md"
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

# 检查笔记是否存在
if [[ ! -f "$MY_NOTES_MD" ]]; then
	echo "找不到笔记: $MY_NOTES_MD" >&2
	exit 1
fi
# 检查历史文件是否存在
if [[ ! -f "$HISTORY_FILE" ]]; then
	echo "找不到历史文件:$HISTORY_FILE" >&2
	exit 1
fi

{
	echo "系统时间:"
	date '+%Y-%m-%d %H:%M:%S %Z'
	echo
	
	echo "最近使用的10条命令"
	tail -n 10 "$HISTORY_FILE"
	echo

	echo "笔记的字数(包含空格和换行)"
	wc -m < "$MY_NOTES_MD"
	echo
} > "$OUTPUT_TXT"
