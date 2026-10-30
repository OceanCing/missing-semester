#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTES_DIR="$SCRIPT_DIR/../notes"
OUT_FILE="$SCRIPT_DIR/output.txt"

NOTE_FILE="$(find "$NOTES_DIR" -maxdepth 1 -type f -name '*.md' 2>/dev/null | sort | head -n 1)"

{
  echo "1. System time"
  date '+%Y-%m-%d %H:%M:%S'

  echo
  echo "2. Last 10 commands"
  tail -n 10 "$HOME/.bash_history"

  echo
  echo "3. Notes word count"
  echo "file       : $(basename "$NOTE_FILE")"
  echo "characters : $(wc -m < "$NOTE_FILE" | tr -d ' ')"
  echo "words      : $(wc -w < "$NOTE_FILE" | tr -d ' ')"
} > "$OUT_FILE"
