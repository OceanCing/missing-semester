#!/bin/bash
{
    echo "运行脚本时的系统时间"
    date
    echo -e "\n最近使用的10条命令"
    tail -n 10 ~/.bash_history
    echo -e "\n笔记字数"
    wc -m < 2027/xujiaming-nb/notes/linux-note.md
} > 2027/xujiaming-nb/homework/output.txt
echo "结果已保存到 output.txt"
