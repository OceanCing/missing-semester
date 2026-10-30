#!/usr/bin/env bash

set -euo pipefail
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
note_file="$script_dir/../notes/lesson1.md"
output_file="$script_dir/output.txt"
history_file="${HISTFILE:-$HOME/.bash_history}"

if [[ ! -f "$note_file" ]]; then
    printf '错误:找不到笔记本文件：%s\n' "$note_file" >&2
    exit 1
fi

{
    printf '运行时间: %s\n\n' "$(date '+%Y-%m-%d %H:%M:%S %Z')"

    printf '最近使用的10条命令:\n'
    if [[ -r "$history_file" && -s "$history_file" ]]; then
        tail -n 10 "$history_file"
    else
        printf '没有可读取的命令历史。请先在终端执行 history -a,再运行脚本。\n'
    fi

    printf '\n笔记字数(字符数):'
    wc -m < "$note_file"
} > "$output_file"

printf '输出已写入:%s\n' "$output_file"
