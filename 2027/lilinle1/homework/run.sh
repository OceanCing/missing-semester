#!/bin/bash

OUTPUT_FILE="$(dirname "$0")/output.txt"
NOTE_FILE="$(dirname "$0")/../notes/lesson1.md"

{
    echo "=== 1. 运行脚本时的系统时间 ==="
    date

    echo -e "\n=== 2. 最近使用的10条命令 ==="
    if [ -f ~/.zsh_history ]; then
        grep -v '^#' ~/.zsh_history | tail -n 10
    elif [ -f ~/.bash_history ]; then
        grep -v '^#' ~/.bash_history | tail -n 10
    else
        HISTFILE=~/.bash_history
        set -o history
        history 10
    fi

    echo -e "\n=== 3. 笔记的字数统计 ==="
    wc -m "$NOTE_FILE"
} > "$OUTPUT_FILE"
