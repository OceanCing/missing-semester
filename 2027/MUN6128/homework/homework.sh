#!/usr/bin/env bash

set -e

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

{
    echo "===== System Time ====="
    date '+%Y-%m-%d %H:%M:%S %Z'

    echo
    echo "===== Recent Shell Commands ====="

    if [[ -f "$HOME/.bash_history" ]]; then
        grep -E '^(git |cd |ls|pwd|mkdir|cat |nano |chmod |find |echo |date|whoami|head |tail |history)' "$HOME/.bash_history" | tail -n 10 || true
    else
        echo "No Bash history file found."
    fi
} > "$OUTPUT_FILE"

echo "Output saved to: $OUTPUT_FILE"
