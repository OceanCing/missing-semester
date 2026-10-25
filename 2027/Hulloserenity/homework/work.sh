#!/bin/bash
echo "运行脚本时的系统时间： $( date '+%Y.%m.%d %H:%M:%S')" > output.txt
echo "最近10条命令：" >> output.txt
tail -n 10 "$HOME/.bash_history" >> output.txt
echo "笔记字数： $(wc -m < ../notes/note1.md)" >>output.txt
