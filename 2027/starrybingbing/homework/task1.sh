#!/bin/bash
NOTE_FILE="../notes/lesson1.md"
{
    echo "===运行脚本时的系统时间==="
    date
    # date查看系统时间，年月日时分
    echo -e "\n===最近使用的10条命令==="
    # -e开启特殊符号识别，\n换行，即先换行再打印“===最近使用的10条命令===”
    history | tail -n 10
    # history查看在这个终端敲过的所有命令，然后送到tail中，-n N指切下最后的N行
    echo -e "\n===笔记的字数==="
    if [ -f "$NOTE_FILE" ]; then
    #-f用来判断这是普通文件吗，then即那么
        wc -m < "$NOTE_FILE"
        #wc为word count的缩写
    else 
        echo "找不到文件"
    fi
    #结束用fi，与if对应
} >output.txt
#重定向到output.txt
echo "运行完成"
