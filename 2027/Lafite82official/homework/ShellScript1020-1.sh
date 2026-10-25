#!/bin/bash 
# 声明使用/bin/bash这个解释器
# 脚本类文件依赖解释器运行，推荐显式指定如 “#!/usr/bin/python3”

# 1. 获取系统时间
echo "系统时间：" > output.txt
# “<”--覆盖写入，如果文件不存在会自动创建；如果存在会覆盖。

date >> output.txt
# ">>" 追加写入，会自动换行。 
# date 命令获取当前时间，不清楚bash脚本是否有像python一样的日期格式化功能。

# 2. 获取最近10条命令
echo "最近使用的10条命令：" >> output.txt
# 跟上面一样，追加一个标题。

tail -n 10 ~/.bash_history >> output.txt
# tail 是“看结尾”的意思，-n 10 表示看最后10行。
# ~/.bash_history 是 Linux 在家目录里默默记录你敲过哪些命令的文件。
# 把最后10条命令追加写入 output.txt。

# 3. 统计笔记字数
echo "笔记字数：" >> output.txt
# 依旧append。

wc -m ../notes/RMNote-1stClassV2.md >> output.txt
# wc 是 Word Count（字数统计）的缩写。
# -m 表示统计【字符数】（包含标点符号和空格）。
# ../notes/RMNote.md 是你笔记文件的路径（../ 表示返回上一级目录，然后再进 notes 文件夹）。

# 由于所有输出都被重定向到output.txt里，终端将无输出。