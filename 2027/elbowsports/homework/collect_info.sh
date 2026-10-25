#!/usr/bin/env bash
# ==============================================================================
# Lecture 1 Homework
#
# Collects three pieces of information and writes them to output.txt in the
# same directory as this script:
#   1. the system time when the script is executed
#   2. the ten most recently used shell commands
#   3. the word count of the notes document
#
# Usage:
#   ./collect_info.sh
#   bash collect_info.sh
#
# The output is always redirected to output.txt; a short confirmation is
# printed on stderr so that stdout stays free for the file.
# ==============================================================================
set -Eeuo pipefail

# --------------------------------------------------------------- paths -------
# Resolve the directory of this script so it works from any working directory.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

# The notes live one level up: homework/ -> <user>/notes/lecture1-notes.md
NOTES_DIR="$(cd "$SCRIPT_DIR/.." && pwd)/notes"
NOTES_FILE="$NOTES_DIR/lecture1-notes.md"

# ------------------------------------------------------------ helpers --------
die() {
    printf 'Error: %s\n' "$*" >&2
    exit 1
}

# Fall back to a different notes file if the default one is missing, so the
# script keeps working when the notes are renamed later in the course.
find_notes_file() {
    if [[ -f "$NOTES_FILE" ]]; then
        return 0
    fi
    local candidate
    candidate="$(find "$NOTES_DIR" -maxdepth 1 -type f -name '*.md' 2>/dev/null | sort | head -n 1)"
    if [[ -n "$candidate" ]]; then
        NOTES_FILE="$candidate"
        return 0
    fi
    die "no Markdown notes found under $NOTES_DIR"
}

# ------------------------------------------------- 1. system time -------------
collect_time() {
    printf '=== System Time ===\n'
    printf 'Date       : %s\n' "$(date '+%Y-%m-%d')"
    printf 'Time       : %s\n' "$(date '+%H:%M:%S')"
    printf 'Timezone   : %s\n' "$(date '+%Z (UTC%:z)')"
    printf 'Unix epoch : %s\n' "$(date '+%s')"
    printf 'Host       : %s\n' "$(uname -n)"
    printf 'Kernel     : %s\n' "$(uname -sr)"
    printf '\n'
}

# ------------------------------------------- 2. last ten commands ------------
# Reading the shell history is environment dependent:
#   * bash writes ~/.bash_history when a session ends
#   * the current session's history is not flushed yet, so `history` is used
#     when the shell is interactive
#   * HISTFILE may point somewhere else entirely
# The function tries the in-memory history first and then the history file, and
# reports clearly when neither yields anything.
collect_commands() {
    printf '=== Last 10 Commands ===\n'

    local -a cmds=()

    # Attempt 1: the current shell's in-memory history (interactive shells).
    if [[ $- == *i* ]] && history 1 >/dev/null 2>&1; then
        mapfile -t cmds < <(history 10 2>/dev/null | sed 's/^ *[0-9]* *//')
    fi

    # Attempt 2: the history file on disk.
    if [[ ${#cmds[@]} -eq 0 ]]; then
        local hist_file="${HISTFILE:-$HOME/.bash_history}"
        if [[ -r "$hist_file" ]]; then
            mapfile -t cmds < <(tail -n 10 "$hist_file" | sed '/^[[:space:]]*$/d')
        fi
    fi

    # Attempt 3: zsh history, in case the user runs zsh.
    if [[ ${#cmds[@]} -eq 0 ]] && [[ -r "$HOME/.zsh_history" ]]; then
        mapfile -t cmds < <(tail -n 10 "$HOME/.zsh_history" \
            | sed 's/^: [0-9]*:[0-9]*;//' | sed '/^[[:space:]]*$/d')
    fi

    if [[ ${#cmds[@]} -eq 0 ]]; then
        printf '(No shell history available in this environment.)\n'
    else
        # Show at most ten entries and number them for readability.
        local i=1
        local cmd
        for cmd in "${cmds[@]}"; do
            ((i > 10)) && break
            printf '%2d. %s\n' "$i" "$cmd"
            ((i++)) || true
        done
    fi

    printf '\n'
}

# --------------------------------------------- 3. notes word count -----------
# Counts words with wc after combining both plain words and CJK characters,
# because the notes contain Chinese text, where wc -w treats a whole sentence
# as a single word.
collect_wordcount() {
    printf '=== Notes Word Count ===\n'
    # Display the path relative to the student folder for readability.
    local student_dir rel_path
    student_dir="$(cd "$SCRIPT_DIR/.." && pwd)"
    rel_path="${NOTES_FILE#"$student_dir"/}"
    printf 'File       : %s\n' "$rel_path"

    # Raw line/word/byte counts reported by wc.
    local lines words bytes
    read -r lines words bytes < <(wc -l -w -c < "$NOTES_FILE" | tr -s ' ')
    printf 'Lines      : %s\n' "$lines"
    printf 'Bytes      : %s\n' "$bytes"

    # wc -w counts whitespace-separated tokens. In a Chinese document a whole
    # sentence glued together counts as one "word", so the number is only
    # meaningful for the Latin parts. The two counts are reported separately
    # instead of being added together, because adding words and characters
    # would not produce a meaningful number.
    printf 'English words : %s\n' "$words"

    # Count CJK characters, because wc -w does not split them.
    # grep exits with status 1 when nothing matches, which under `set -e` and
    # `pipefail` would abort the whole script, so the failure is neutralised
    # with `|| true` before piping into wc.
    local cjk=0
    cjk="$( { grep -oP '[\x{4e00}-\x{9fff}\x{3040}-\x{30ff}\x{ac00}-\x{d7af}]' \
        "$NOTES_FILE" 2>/dev/null || true; } | wc -l)"
    cjk="${cjk//[[:space:]]/}"
    [[ "$cjk" =~ ^[0-9]+$ ]] || cjk=0
    printf 'Chinese chars : %s\n' "$cjk"

    printf '\n'
}

# ------------------------------------------------------------- main ----------
main() {
    find_notes_file

    # Write everything into output.txt. Grouping keeps a single redirection and
    # guarantees the file is truncated before anything is written.
    {
        printf 'Lecture 1 Homework Output\n'
        printf 'Generated by: %s\n' "${USER:-$(id -un)}"
        printf 'Script      : %s\n' "${BASH_SOURCE[0]##*/}"
        printf '\n'
        collect_time
        collect_commands
        collect_wordcount
    } > "$OUTPUT_FILE"

    printf 'Report written to %s\n' "$OUTPUT_FILE" >&2
}

main "$@"
