#!/bin/bash

{
    date
    history 10
    wc -w ../note/notes.md
} > output.txt
