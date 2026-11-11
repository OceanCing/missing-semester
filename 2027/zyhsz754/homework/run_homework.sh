#!/bin/bash
echo "=== 时间 ==="
date
echo "=== 命令 ==="
history | tail -n 10
echo "=== 字数 ==="
wc -m ../notes/lesson01.md
