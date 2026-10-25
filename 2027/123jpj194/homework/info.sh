#!/bin/bash
# 第一行是特殊注释，“#！”组合能指定解释器，此处起到告诉系统用bash来执行脚本，必须在首行

# 获取脚本所在目录，用于定位笔记文件和输出文件
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTE_FILE="$SCRIPT_DIR/../notes/lesson1.md"
OUTPUT_FILE="$SCRIPT_DIR/output.txt"

# ${BASH_SOURCE[0]}：脚本自己的路径
# dirname：取出路径里的目录部分
# cd ... && pwd：进入该目录并打印绝对路径
# $SCRIPT_DIR/../notes/lesson1.md：从 homework 往上一级，再进入 notes，得到笔记文件（homework往上一级是123jpj194,我的文件夹）
# $( ... ) 的意思是"执行里面的命令，把输出当作值"
# 文字描述为：先获得脚本自己的路径，再取出所得路径的目录，进入该目录并打印绝对路径，组合成完整路径，结果赋值给SCRIPT_DIR
# OUTPUT_FILE用于重定向

{ 
# 输出运行脚本时的系统时间
echo "运行脚本的系统时间：" 
date "+%Y-%m-%d %H:%M:%S"
echo

# 输出最近的10条命令
echo "最近使用的10条命令"
tail -n 10 ~/.bash_history
echo
# 在终端输入 history 能得到这个终端里敲过的所有命令，想要得到最后10条需要写成 history | tail -n 10 ，用到了管道
# 然而直接在脚本里写 history | tail -n 10 执行时不会有结果，因为脚本里没有终端的命令记录
# 我们需要知道 bash 会把命令历史文件保存到文件 ～/.bash_history ，所以想在脚本里获得前10条命令应该这样写：tail -n 10 ~/.bash_history
# 值得注意，bash默认在关闭终端时才把本次会话的命令写入 ~/.bash_history ，所以直接写 tail -n 10 ~/.bash_history 得到的结果是上次关闭终端前的命令记录
# 所以在写脚本前，如果想要获得当前终端的命令记录，需要进行 history -a 的操作，-a可以把本次会话新增的命令加入历史文件里，但是在脚本里写 history -a 没用
# 想一劳永逸怎么办？可以在终端里这样写：
# echo 'PROMPT_COMMAND="history -a; $PROMPT_COMMAND"' >> ~/.bashrc
# source ~/.bashrc 
# 即每敲完一条命令就自动写入文件

# 输出笔记字符数
echo "笔记字数："
wc -m < "$NOTE_FILE"
# wc 是 “word count” ，常用选项有：
# -m：字符数（一个汉字算一个)
# -w：单词数（按空格分隔）
# -l：行数
# -c：字节数
# “<”号和先前学的“>”号相反，前者是把文件内容送给命令当作输入，后者是把输出送给文件
# “<”在这的作用是把文件重定向为输入，这样输出就只有字数而没有文件名了，更美观

} > "$OUTPUT_FILE"
