#!/usr/bin/env bash
set -u

# 脚本所在目录，也就是 home word 文件夹
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 笔记所在目录；如果 Markdown 笔记就在 home word 文件夹下，不用改
NOTE_DIR="$SCRIPT_DIR/../notes"

# 输出文件，固定写到脚本同目录下
OUTPUT="$SCRIPT_DIR/output.txt"

# 统计所有 .md 笔记的字数（字符数，中文每个字算 1 个字符）
# 排除 output.txt；先统计再写文件，避免把输出文件算进去
word_count="$(find "$NOTE_DIR" -type f -name '*.md' ! -name 'output.txt' -exec cat {} + | wc -m | tr -d '[:space:]')"

# 最近 10 条命令：脚本里直接 history 通常拿不到，改读历史文件
HIST_FILE="${HISTFILE:-$HOME/.bash_history}"
if [ ! -r "$HIST_FILE" ] && [ -r "$HOME/.zsh_history" ]; then
  HIST_FILE="$HOME/.zsh_history"
fi
recent_cmds="$(tail -n 10 "$HIST_FILE" 2>/dev/null || true)"
[ -n "$recent_cmds" ] || recent_cmds="（无法读取命令历史文件）"

# 统一重定向到同路径 output.txt
{
  echo "运行时间：$(date '+%Y-%m-%d %H:%M:%S')"
  echo
  echo "最近使用的10条命令："
  echo "$recent_cmds"
  echo
  echo "笔记字数（Markdown 字符数，含标点/换行）：$word_count"
} > "$OUTPUT"

echo "已写入：$OUTPUT"