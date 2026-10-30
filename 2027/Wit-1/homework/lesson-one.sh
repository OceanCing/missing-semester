#!/usr/bin/env bash
# 运行前在原来的 Bash 终端执行 history -a，保存本次会话的历史。
set -euo pipefail
export LC_ALL=C.UTF-8

script_dir=$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
note_file="$script_dir/../notes/lesson-one.md"
history_file=${HISTFILE:-"$HOME/.bash_history"}
output_file="$script_dir/output.txt"

if [[ ! -f "$note_file" || ! -r "$note_file" ]]; then
    printf '无法读取笔记：%s\n' "$note_file" >&2
    exit 1
fi

if [[ ! -f "$history_file" || ! -r "$history_file" || ! -s "$history_file" ]]; then
    printf '历史文件为空或不可读：%s\n请在 Bash 终端先执行 history -a。\n' "$history_file" >&2
    exit 1
fi

# history -r 支持 Bash 历史文件中的时间戳和多行命令。
# 此处只修改脚本自身的内存历史，不修改原终端或历史文件。
history -c
history -r "$history_file"
recent_commands=$(HISTTIMEFORMAT= history 10)
if [[ -z "$recent_commands" ]]; then
    printf '历史文件中没有可输出的命令。\n' >&2
    exit 1
fi

{
    printf '运行时系统时间：\n'
    date '+%Y-%m-%d %H:%M:%S %Z %z'
    printf '\n最近使用的 10 条命令（不足 10 条时输出全部）：\n'
    printf '%s\n' "$recent_commands"
    printf '\n笔记：notes/lesson-one.md\n'
    printf '笔记字符数（wc -m，含空白和换行）：'
    wc -m < "$note_file"
    printf '空白分隔词数（wc -w）：'
    wc -w < "$note_file"
} > "$output_file"

printf '已生成：%s\n' "$output_file"
