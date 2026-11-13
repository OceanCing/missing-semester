
#!/usr/bin/env bash
set -euo pipefail

# Locate the files relative to this script.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTES_FILE="$SCRIPT_DIR/../notes/linux-git-basics.md"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

# Check whether the Markdown file exists.
if [[ ! -f "$NOTES_FILE" ]]; then
    echo "Error: Markdown notes not found." >&2
    exit 1
fi

# Redirect all report content into output.txt.
{
    echo "===== System Report ====="
    echo "Current time: $(date '+%Y-%m-%d %H:%M:%S %Z')"
    echo

    echo "===== Last 10 Commands ====="
    if [[ -f "$HISTORY_FILE" ]]; then
        tail -n 10 "$HISTORY_FILE"
    else
        echo "No saved Bash history found."
    fi
    echo

    echo "===== Markdown Notes ====="
    echo "File: $(basename "$NOTES_FILE")"
    echo "Character count: $(wc -m < "$NOTES_FILE")"
} > "$OUTPUT_FILE"

echo "Report generated: $OUTPUT_FILE"
