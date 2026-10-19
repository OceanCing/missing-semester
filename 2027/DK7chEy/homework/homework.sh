#!/usr/bin/env bash

DIR="$(cd "$(dirname "$0")" && pwd)"
OUT="$DIR/output.txt"

echo "===== 1. 系统时间 =====" > "$OUT"
date >> "$OUT"

echo "" >> "$OUT"
echo "===== 2. 最近使用的 10 条命令 =====" >> "$OUT"
tail -n 10 ~/.bash_history >> "$OUT"

echo "" >> "$OUT"
echo "===== 3. 笔记字数统计 =====" >> "$OUT"
wc -m "$DIR/../notes/"*.md >> "$OUT"


