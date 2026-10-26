#!/usr/bin/env bash
set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
NOTES_DIR="$(dirname -- "$SCRIPT_DIR")/notes"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

UTF8_LOCALE="$(locale -a 2>/dev/null | grep -ix 'C\.utf-\?8'    | head -n 1)"
[[ -n "$UTF8_LOCALE" ]] || \
UTF8_LOCALE="$(locale -a 2>/dev/null | grep -ix 'zh_CN\.utf-\?8' | head -n 1)"

{
    echo "系统时间"
    date '+%Y-%m-%d %H:%M:%S %Z (UTC%z)'

    echo
    echo "最近10条命令"
    HIST_FILE="${HISTFILE:-$HOME/.bash_history}"
    [[ -r "$HIST_FILE" ]] || HIST_FILE="$HOME/.zsh_history"
    tail -n 10 -- "$HIST_FILE"

    total_chars=0
    
    NOTE_FILES=()
    mapfile -d '' -t NOTE_FILES < <(
        find "$NOTES_DIR" -type f -name '*.md' -print0 | sort -z
    )
    for f in "${NOTE_FILES[@]}"; do
        read -r chars <<< "$(LC_ALL="$UTF8_LOCALE" wc -m < "$f")"
        total_chars=$((total_chars + chars))
        echo
        echo "笔记字数：$chars （$f）"
    done
} > "$OUTPUT_FILE"