#!/bin/bash

notes_path="../notes/notesone.md"

cat>output.txt<<EOF

$(date)

$(tail -n 10 ~/.bash_history)

$(wc -m < "$notes_path")

EOF


