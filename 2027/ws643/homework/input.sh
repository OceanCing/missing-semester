#!/bin/bash


echo "运行时间:" > output.txt
date >> output.txt
echo "" >> output.txt


echo "最近10条命令:" >> output.txt
history | tail -n 10 >> output.txt
echo "" >> output.txt


echo "笔记字数:" >> output.txt
wc -m ~missing-semester/2027/notes/notes.md >> output.txt