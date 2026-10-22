#!/bin/bash

# Set the output file path
OUTPUT="2027/wskssg31/homework/output.txt"
NOTE="2027/wskssg31/notes/lesson1-notes.md"

# Redirect the report to output.txt
{
    echo "System time:"
    date

    echo
    echo "Last 10 commands:"
    tail -n 10 "$HOME/.bash_history"

    echo
    echo "Word count of lesson1-notes.md:"
    wc -w < "$NOTE"
} > "$OUTPUT"
