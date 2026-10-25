#!/bin/bash
echo "=== System Time ===" > output.txt
date >> output.txt
echo -e "\n=== Recent 10 Commands ===" >> output.txt
history | tail -n 10 >> output.txt
echo -e "\n=== Note Word Count ===" >> output.txt
wc -w ../notes/linux_git_basics.md >> output.txt
