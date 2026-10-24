touch output.txt

NOTE_FILE="../notes/the_first_lesson.md"
OUTPUT="./output.txt"

echo "date:" > "$OUTPUT"
date >> "$OUTPUT"

echo "history 10:" >> "$OUTPUT"
history 10 >> "$OUTPUT"

echo "notes_count:" >> "$OUTPUT"
wc -m "$NOTE_FILE" >> "$OUTPUT"