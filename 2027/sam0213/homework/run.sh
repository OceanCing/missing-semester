#!bin/bash
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUTPUT="$SCRIPT_DIR/output.txt"
NOTE_FILE="$SCRIPT_DIR/../notes/lesson1.md"

{
	echo "----system time-----"
	date "+%Y-%m-%d %H:%M:%S"
	echo ""
	echo "--ten command recently---"
	if [ -f "$HOME/.bash_history" ]; then
		tail -n 10 "$HOME/.bash_history"
	else
		history 10 | tail -n 10
	fi
	echo ""
	echo "--notes---"
    if [ -f "$NOTE_FILE" ]; then
	   echo "word count: $(wc -w < "$NOTE_FILE")"
	   echo "char count: $(wc -m < "$NOTE_FILE")"
	else
		echo "note file not found: $NOTE_FILE"
	fi
}	> "$OUTPUT"


echo "Result saved to $OUTPUT"
