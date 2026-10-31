#!/usr/bin/env bash

# 获取脚本所在目录，避免从其他目录运行时找不到文件
script_dir="$(cd "$(dirname "$0")" && pwd)"
notes_dir="${script_dir}/../notes"
output_file="${script_dir}/output.txt"

if [ ! -d "$notes_dir" ]; then
    echo "未找到笔记目录：$notes_dir"
    exit 1
fi

shopt -s nullglob
note_files=("$notes_dir"/*.md)

if [ ${#note_files[@]} -eq 0 ]; then
    echo "笔记目录中没有 Markdown 文件。"
    exit 1
fi

{
    echo "运行时间：$(date '+%Y-%m-%d %H:%M:%S %Z')"
    echo
    echo "最近使用的10条命令："

    if [ -f "$HOME/.bash_history" ]; then
        # Bash 开启时间戳后会出现以 # 开头的时间戳行，这里把它们去掉
        grep -Ev '^#[0-9]{10,}$' "$HOME/.bash_history" | tail -n 10
    elif [ -f "$HOME/.zsh_history" ]; then
        # 去掉 Zsh 历史记录前面的时间戳
        sed -E 's/^: [0-9]+:[0-9]+;//' "$HOME/.zsh_history" | tail -n 10
    else
        echo "没有找到可读取的命令历史文件。"
    fi

    echo
    echo "笔记文件数量：${#note_files[@]}"
    echo "笔记总字数：$(cat "${note_files[@]}" | tr -d '[:space:]' | wc -m)"
} > "$output_file"

echo "结果已写入：$output_file"
