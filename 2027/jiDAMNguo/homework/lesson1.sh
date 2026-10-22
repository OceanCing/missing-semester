#!/usr/bin/env bash
#
# Lesson 1 homework script.
#
# It prints:
#   1. the system time when the script runs
#   2. the last 10 shell commands
#   3. the character count and word count of the notes
#
# All output is redirected to output.txt in the same directory as this script.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
NOTES_DIR="$SCRIPT_DIR/../notes"

# Find the first Markdown note in ../notes so the note file name does not
# have to be hard-coded here.
NOTES_FILE="$(find "$NOTES_DIR" -maxdepth 1 -type f -name '*.md' 2>/dev/null | sort | head -n 1)"

# A script started from a non-interactive shell cannot use the `history`
# builtin, so read the history file that bash writes on exit instead.
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

{
    echo "Lesson 1 homework output"
    echo "========================="
    echo
    echo "System time:"
    date '+%Y-%m-%d %H:%M:%S %Z'
    echo
    echo "Last 10 commands:"
    if [ -r "$HISTORY_FILE" ]; then
        tail -n 10 "$HISTORY_FILE"
    else
        echo "(command history is not available)"
    fi
    echo
    echo "Notes word count:"
    if [ -n "$NOTES_FILE" ] && [ -r "$NOTES_FILE" ]; then
        echo "file: $(basename "$NOTES_FILE")"
        echo "characters (wc -m): $(wc -m < "$NOTES_FILE" | tr -d ' ')"
        echo "words (wc -w): $(wc -w < "$NOTES_FILE" | tr -d ' ')"
    else
        echo "(notes file not found in $NOTES_DIR)"
    fi
} > "$OUTPUT_FILE"
