#!/bin/bash
HISTFILE=~/.bash_history
set -o history

NOTE_FILE="../notes/TASK.md"

echo "==== Current System Time ====" > output.txt
date >> output.txt

echo -e "\n==== Recent 10 Commands ====" >> output.txt
history | tail -n 10 >> output.txt

echo -e "\n==== Word count of note file ====" >> output.txt
wc -w "$NOTE_FILE" >> output.txt

