#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
NOTES_FILE="$SCRIPT_DIR/../notes/linux-vim-git-notes.md"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"
{
	echo "===== System Time ====="
	date
	echo ""
	echo "===== Last 10 Commands ====="
	tail -n 10 ~/.bash_history
	echo ""
	echo "===== Notes Word Count ====="
	CHAR_COUNT=$(wc -m < "$NOTES_FILE")
	echo "Characters: $CHAR_COUNT"
} > "$OUTPUT_FILE"
echo "Done! Output saved to output.txt"
