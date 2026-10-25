#!/bin/bash
echo "脚本运行时的系统时间：" > output.txt
date >> output.txt
echo "最近使用的10条命令：" >> output.txt
history 10 >> output.txt
echo "课程一笔记的字数：" >> output.txt
wc -m ../notes/Class1.md >> output.txt
