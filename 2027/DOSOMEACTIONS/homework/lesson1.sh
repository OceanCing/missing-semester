#!/bin/bash
# ============================================================
# 培训课第一课作业脚本
# 输出：① 运行时的系统时间  ② 最近使用的 10 条命令  ③ 笔记字数
#      全部重定向到本脚本同目录下的 output.txt
# 用法：./lesson1.sh        然后看 output.txt
# ============================================================

# 拿到脚本所在目录（不管从哪调用都能正确定位）
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT="$SCRIPT_DIR/output.txt"
NOTES="$SCRIPT_DIR/../notes/lesson1-linux-vim-shell-git.md"

# 整个代码块的花括号 + > 重定向：里面所有 echo 都写进 output.txt
{
    echo "=========================================="
    echo "培训课第一课作业 · 运行结果"
    echo "=========================================="
    echo

    # ---------- ① 系统时间 ----------
    echo "【1】运行脚本时的系统时间"
    echo "------------------------------------------"
    date '+%Y-%m-%d %H:%M:%S %Z (星期%u)'
    echo

    # ---------- ② 最近 10 条命令 ----------
    echo "【2】最近使用的 10 条命令"
    echo "------------------------------------------"
    # 注意：非交互式 shell 里 history 是空的，
    # 必须直接读 ~/.bash_history 文件
    if [ -r "$HOME/.bash_history" ]; then
        tail -n 10 "$HOME/.bash_history" | nl -ba -w2 -s'. '
    else
        echo "(无法读取命令历史：$HOME/.bash_history 不存在)"
    fi
    echo

    # ---------- ③ 笔记字数 ----------
    echo "【3】笔记字数"
    echo "------------------------------------------"
    if [ -f "$NOTES" ]; then
        echo "文件路径: ${NOTES#$SCRIPT_DIR/../}"
        # wc -m 统计字符数（对中文正确）；-c 是字节数，不要用
        CHARS=$(wc -m < "$NOTES" | tr -d ' ')
        LINES=$(wc -l < "$NOTES" | tr -d ' ')
        echo "字符数:   $CHARS"
        echo "行数:     $LINES"
    else
        echo "(笔记文件不存在: $NOTES)"
    fi
    echo

    echo "=========================================="
    echo "输出结束"
    echo "=========================================="
} > "$OUTPUT"

# 脚本本身在终端打印一行提示（不写进 output.txt）
echo "已生成: $OUTPUT"
