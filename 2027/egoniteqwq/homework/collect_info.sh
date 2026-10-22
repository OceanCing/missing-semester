#!/usr/bin/env bash
#
# Collect homework information and write it to output.txt next to this script.
#
# Output content:
#   1. The system time when the script is executed.
#   2. The last 10 commands from the current user's shell history.
#   3. The character/word count of the Markdown notes.
#
# Usage: ./collect_info.sh

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
NOTES_DIR="$SCRIPT_DIR/../notes"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
HISTORY_FILE="${HISTFILE:-$HOME/.bash_history}"

# Print the last 10 shell commands. A script runs in a non-interactive shell,
# so load the history file explicitly before asking bash for recent entries.
print_last_commands() {
    local recent=""

    if [ -n "${BASH_VERSION:-}" ] && [ -r "$HISTORY_FILE" ]; then
        history -r "$HISTORY_FILE" 2>/dev/null || true
        recent="$(history 10 2>/dev/null | tail -n 10)"
    fi

    if [ -z "$recent" ] && [ -r "$HISTORY_FILE" ]; then
        recent="$(tail -n 10 "$HISTORY_FILE")"
    fi

    if [ -z "$recent" ]; then
        recent="(no shell history available)"
    fi

    printf '%s\n' "$recent"
}

# Count characters (whitespace excluded) and words in all Markdown notes.
count_notes() {
    local -a note_files=()
    local file chars words

    while IFS= read -r -d '' file; do
        note_files+=("$file")
    done < <(find "$NOTES_DIR" -type f -name '*.md' -print0 2>/dev/null | sort -z)

    if [ "${#note_files[@]}" -eq 0 ]; then
        printf '0 0\n'
        return
    fi

    chars="$(cat "${note_files[@]}" | tr -d '[:space:]' | wc -m)"
    words="$(cat "${note_files[@]}" | wc -w)"
    printf '%s %s\n' "$chars" "$words"
}

note_count="$(count_notes)"
note_chars="${note_count%% *}"
note_words="${note_count##* }"

{
    printf '===== Homework Output =====\n'
    printf 'Script path : %s\n' "$SCRIPT_DIR"
    printf 'System time : %s\n' "$(date '+%Y-%m-%d %H:%M:%S %Z (%z)')"
    printf '\n'

    printf '%s\n' '----- Last 10 commands -----'
    print_last_commands
    printf '\n'

    printf '%s\n' '----- Notes word count -----'
    printf 'Notes directory : %s\n' "$NOTES_DIR"
    printf 'Characters (without whitespace): %s\n' "$note_chars"
    printf 'Words (wc -w): %s\n' "$note_words"
} > "$OUTPUT_FILE" 2>&1

printf 'Output written to %s\n' "$OUTPUT_FILE" >&2
