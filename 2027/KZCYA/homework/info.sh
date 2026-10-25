#!/bin/bash
{
    echo 运行脚本的系统时间
    date
    echo -e "\n最近使用的十条命令"
    cat ~/.bash_history|tail -10
    echo -e "\n笔记字数"
    wc ../notes/note1.md
}>output.txt