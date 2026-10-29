#!/bin/bash

OUTPUT_FILE="2027/XingStar-spec/homework/output.text"
NOTES_FILE="2027/XingStar-spec/notes/lesson1_notes.md"
> "$OUTPUT_FILE"
echo "===系统时间===" >> "$OUTPUT_FILE"
date >> "$OUTPUT_FILE"

echo " " >> "$OUTPUT_FILE"
echo "===最近使用的10条命令===" >> "$OUTPUT_FILE"
tail -n 10 ~/.bash_history >> "$OUTPUT_FILE"


echo " " >> "$OUTPUT_FILE"
echo "===笔记字数统计===" >> "$OUTPUT_FILE"
if [ -f "$NOTES_FILE" ]; then
    wc -m "$NOTES_FILE" >> "$OUTPUT_FILE"
else 
    echo "为找到笔记" >> "$OUTPUT_FILE"
fi

echo "$OUTPUT_FILE"
