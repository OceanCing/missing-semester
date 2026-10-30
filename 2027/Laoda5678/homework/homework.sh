#!/bin/bash

set -o history

{
    echo "当前系统时间"
    date
    
    echo ""
    echo "最近使用的 10 条命令"
    tail -n 10 ~/.bash_history
} > output.txt

echo "脚本运行完成,结果已保存到 output.txt 中。"

