#!/bin/bash
{
# Get current time
date
# Last 10 history commands
tail -n 10 ~/.bash_history
# Word count of lesson1.md
wc -w ../notes/lesson1.md
} > output.txt

