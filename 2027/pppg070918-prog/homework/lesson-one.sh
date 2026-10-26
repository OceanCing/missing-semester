#!/usr/bin/env bash
set -uo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
notes_file="$script_dir/../notes/lesson-one.md"
output_file="$script_dir/output.txt"
history_file="${HISTFILE:-$HOME/.bash_history}"

{
    printf '系统时间：%s\n\n' "$(date '+%Y-%m-%d %H:%M:%S %Z (%z)')"

    printf '最近使用的 10 条命令：\n'
    if [[ -r "$history_file" ]]; then
        sed \
            -e '/^\x1B\[200~/ { s/\x1B\[200~//; s/~$//; }' \
            -e 's/\x1B\[201~//g' \
            -e '/^[[:space:]]*$/d' \
            "$history_file" |
            tail -n 10
    else
        printf '（未找到命令历史文件：%s）\n' "$history_file"
    fi

    if [[ -r "$notes_file" ]]; then
        char_count="$(LC_ALL=C.UTF-8 wc -m < "$notes_file" | tr -d '[:space:]')"
        compact_count="$(
            LC_ALL=C.UTF-8 tr -d '[:space:]' < "$notes_file" |
                wc -m |
                tr -d '[:space:]'
        )"
        word_count="$(LC_ALL=C.UTF-8 wc -w < "$notes_file" | tr -d '[:space:]')"

        printf '\n笔记字数（按字符计，含空白）：%s\n' "$char_count"
        printf '笔记字数（按字符计，不含空白）：%s\n' "$compact_count"
        printf '笔记词数（按空白分隔）：%s\n' "$word_count"
    else
        printf '\n未找到笔记文件：%s\n' "$notes_file"
    fi
} > "$output_file"
