#!/bin/bash
t1=$(date)
t2="接下来要输出的是shell的前10条指令"
t3=$(tail -n 10 ~/.bash_history)
t4="接下来要输出的是note.md的字数"
t5=$(wc -m < ../notes/my_note.md)
t6="接下来要输出的是c++_note.md的字数"
t7=$(wc -m < ../notes/c++_note.md )
{
printf '%s\n' "$t1"
printf '%s\n' "$t2"
printf '%s\n' "$t3"
printf '%s\n' "$t4"
printf '%s\n' "$t5"
printf '%s\n' "$t6"
printf '%s\n' "$t7"
} > output.txt

