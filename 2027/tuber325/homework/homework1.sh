#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
NOTE="$SCRIPT_DIR/../notes/lesson1-linux-vim-git.md"
OUT="$SCRIPT_DIR/output.txt"

{
  echo "=== System Time ==="
  date "+%Y-%m-%d %H:%M:%S"
  echo

  echo "=== Last 10 Commands ==="
  tail -n 10 ~/.bash_history
  echo

  echo "=== Note Character Count ==="
  wc -m < "$NOTE"
} > "$OUT"
