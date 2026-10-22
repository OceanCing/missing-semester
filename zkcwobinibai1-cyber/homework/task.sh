#!/bin/bash

echo "==== Current system time ===="
date

echo -e "\n==== Last 10 history commands ===="
history | tail -n 10

echo -e "\n==== Word count of note ===="
wc -w ../notes/linux-vim-shell-git.md

