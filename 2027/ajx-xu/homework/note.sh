#!/bin/bash

OUTPUT="output.txt"
>"$OUTPUT"
echo "系统时间:" >> "$OUTPUT"
date >> "$OUTPUT"

echo "历史指令(10):" >> "$OUTPUT"
tail -n 10 ~/.bash_history >> "$OUTPUT"

echo "笔记字数:" >> "$OUTPUT"
wc -w ../notes/lesson1_note.md >> "$OUTPUT"

