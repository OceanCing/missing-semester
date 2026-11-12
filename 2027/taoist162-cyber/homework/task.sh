#!/bin/bash

echo "===== Current System Time ====="
date

echo -e "\n===== Last 10 commands ====="
tail -n 10 ~/.bash_history

NOTE_FILE="../notes/lesson_1.md"
echo -e "\n===== Word count of note file ====="
tr -d '\n ' < "${NOTE_FILE}" | wc -m

{
echo "===== Current System Time ====="
date
echo -e "\n===== Last 10 commands ====="
tail -n 10 ~/.bash_history
echo -e "\n===== Word count of note file ====="
tr -d '\n ' < "${NOTE_FILE}" | wc -m
} > output.txt

