#!/usr/bin/env bash
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
note_dir="$(cd -- "${script_dir}/../notes" && pwd)"
note_file="${note_dir}/lesson1-notes.md"
history_file="${HISTFILE:-$HOME/.bash_history}"

{
	echo "time:"
	date
	echo

	echo "command:"
	tail -n 10 "$history_file"
	echo

	echo "words: $(wc -w < "$note_file")"
	echo "characters: $(wc -m < "$note_file")"
} > "${script_dir}/output.txt"

