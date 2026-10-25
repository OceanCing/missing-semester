#!/bin/bash

# 获取当前系统时间
echo "当前系统时间: $(date)"

# 获取最近使用的10条命令
echo "最近使用的10条命令:"
history | tail -n 10

# 统计刚才写的笔记的字数 (字符数)
# 注意: 这里统计的是字符数, 如果想统计英文单词数可以把 -m 改成 -w
NOTE_FILE="../notes/class1_notes.md"

if [ -f "$NOTE_FILE" ]; then
    CHAR_COUNT=$(wc -m < "$NOTE_FILE")
    echo "你的笔记字数: $CHAR_COUNT"
else
    echo "未找到笔记文件，请检查路径。"
fi
