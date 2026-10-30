#!/usr/bin/env bash

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
notes_file="$script_dir/../notes/lesson1-notes.md"
output_file="$script_dir/output.txt"

{
  echo "Run time: $(date '+%Y-%m-%d %H:%M:%S %Z')"
  echo
  echo "Last 10 commands:"
  if [ -n "$(history 1 2>/dev/null)" ]; then
    history 10
  else
    tail -n 10 "${HISTFILE:-$HOME/.bash_history}" 2>/dev/null
  fi
  echo
  echo "Notes characters: $(wc -m < "$notes_file")"
  echo "Notes words: $(wc -w < "$notes_file")"
} > "$output_file"

echo "Output written to $output_file"
