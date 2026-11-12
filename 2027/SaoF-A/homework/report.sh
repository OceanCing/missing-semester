#!/usr/bin/env bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
NOTE_FILE="$SCRIPT_DIR/../notes/lesson1.md"
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

printf 'System time: '
date '+%Y-%m-%d %H:%M:%S %Z'

printf '\nLast 10 commands:\n'
if [[ -f "$HISTORY_FILE" ]]; then
    tail -n 10 "$HISTORY_FILE"
else
    printf 'History file not found: %s\n' "$HISTORY_FILE"
fi

printf '\nNote character count: '
wc -m < "$NOTE_FILE"
