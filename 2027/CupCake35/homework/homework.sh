#!/usr/bin/env bash
# Lesson 1 homework script.
# Outputs: 1) current system time  2) last 10 commands  3) notes word count.
# All output is redirected with `>` into output.txt in the SAME directory
# as this script (works no matter which directory you run it from).
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# 固定统计第一课的笔记文件。
# 注意：notes/ 里有多篇笔记时，"自动找第一个 .md" 会按文件名排序挑错文件，所以这里写死。
NOTES_FILE="$SCRIPT_DIR/../notes/lesson01-notes.md"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

main() {
    echo "=== Current System Time ==="
    date "+%Y-%m-%d %H:%M:%S %Z (%z)"

    echo
    echo "=== Last 10 Commands ==="
    # In a non-interactive script the builtin `history` is unavailable,
    # so read the shell history file directly.
    if [[ -n "${HISTFILE:-}" && -r "$HISTFILE" ]]; then
        tail -n 10 "$HISTFILE"
    elif [[ -r "$HOME/.bash_history" ]]; then
        tail -n 10 "$HOME/.bash_history"
    elif [[ -r "$HOME/.zsh_history" ]]; then
        tail -n 10 "$HOME/.zsh_history"
    else
        echo "(shell history file not found)"
    fi

    echo
    echo "=== Notes Word Count ==="
    if [[ -r "$NOTES_FILE" ]]; then
        echo "Characters (wc -m): $(wc -m < "$NOTES_FILE" | tr -d ' ')"
        echo "Words (wc -w):      $(wc -w < "$NOTES_FILE" | tr -d ' ')"
    else
        echo "ERROR: notes file not found: $NOTES_FILE" >&2
        return 1
    fi
}

# 屏幕上提示一下用的是哪个笔记文件（写到 stderr，不会混进 output.txt）
printf '笔记文件：%s\n' "${NOTES_FILE:-(未找到 .md 文件)}" >&2

# 用 > 把脚本的标准输出重定向到同目录下的 output.txt（覆盖写入）
main > "$OUTPUT_FILE"

# 文件不需要在屏幕上显示内容，这里只给一句结果提示（写到 stderr，不会混进 output.txt）
printf '已生成：%s\n' "$OUTPUT_FILE" >&2
