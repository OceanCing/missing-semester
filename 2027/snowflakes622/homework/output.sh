#!/bin/bash

OUTPUT_FILE="./output.txt"

> $OUTPUT_FILE

echo "==== 系统时间 ====" >> $OUTPUT_FILE
date >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE

echo "==== 最近使用的10条命令 ====" >> $OUTPUT_FILE
history | tail -n 10 >> $OUTPUT_FILE
echo "" >> $OUTPUT_FILE

NOTES_FILE="../notes/class1_note01.md"
echo "==== 笔记的字数 ====" >> $OUTPUT_FILE
wc -w < $NOTES_FILE >> $OUTPUT_FILE
