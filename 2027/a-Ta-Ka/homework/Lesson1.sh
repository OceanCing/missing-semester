#!/bin/bash

HW="$(dirname "$0")"

output="$HW/output.txt"

note="$HW/../notes/Lesson1.md"

{
    date
    tail -n 10 ~/.bash_history
    wc -m < "$note"
} > "$output"
    

