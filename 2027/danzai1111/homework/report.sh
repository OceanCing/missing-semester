#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
NOTES_FILE="$SCRIPT_DIR/../notes/linux-shell-git.md"

{
    echo "Current system time:"
    date

    echo ""
    echo "Last 10 commands:"
    tail -n 10 ~/.bash_history

    echo ""
    echo "Word count of notes:"
    wc -w < "$NOTES_FILE"
} > "$OUTPUT_FILE"

cat "$OUTPUT_FILE"
