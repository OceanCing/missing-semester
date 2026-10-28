#!/bin/bash
#脚本运行的时间
date > output.txt
#最近的命令
tail -n 10 ~/.bash_history >> output.txt
#字数
wc -m ../notes/firstclass.md >> output.txt
