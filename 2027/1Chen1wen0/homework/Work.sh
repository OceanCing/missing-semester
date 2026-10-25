#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
NOTES_FILE="$SCRIPT_DIR/../notes/training-01-notes.md"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

{
    echo "System time: $(date '+%Y-%m-%d %H:%M:%S')"
    echo
    echo "Last 10 commands:"
    HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

if [[ -r "$HISTORY_FILE" ]]; then
    tail -n 10 "$HISTORY_FILE"
else
    echo "History unavailable"
fi
    echo
    echo "Note word count:"
    wc -w < "$NOTES_FILE"
} > "$OUTPUT_FILE"
