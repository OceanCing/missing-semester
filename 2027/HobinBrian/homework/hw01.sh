#!/usr/bin/env bash
#
# 27赛季控制视觉招新培训 · 第一课作业
# 功能：运行后在终端输出三项信息，并重定向保存到脚本同目录下的 output.txt
#   1. 运行脚本时的系统时间
#   2. 最近使用的 10 条命令
#   3. 笔记的字数
#
# 用法：
#   bash hw01.sh
# 运行后会在 homework/ 目录下生成 output.txt
#
set -uo pipefail

# ---------- 路径准备 ----------
# 用 $(dirname "$0") 定位脚本所在目录，这样从任何位置运行都不会找不到文件
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_FILE="$SCRIPT_DIR/output.txt"
NOTES_DIR="$SCRIPT_DIR/../notes"

# 在 notes 目录下寻找英文命名的 .md 笔记（取第一个）
NOTE_FILE="$(find "$NOTES_DIR" -maxdepth 1 -type f -name '*.md' 2>/dev/null | sort | head -n 1)"

# ---------- 报表内容 ----------
print_report() {
    echo "=================================================="
    echo " 27赛季控制视觉招新培训 · 第一课作业"
    echo " 作者：HobinBrian"
    echo "=================================================="
    echo

    # ---- 1. 系统时间 ----
    echo "[1] 运行脚本时的系统时间"
    echo "--------------------------------------------------"
    # 有些环境（如 Git Bash）%Z 会返回空白，先去掉空白再兜底
    NOW="$(date '+%Y-%m-%d %H:%M:%S')"
    ZONE="$(date '+%Z' | tr -d '[:space:]')"
    [ -z "$ZONE" ] && ZONE="Local"
    OFFSET="$(date '+%z')"
    echo "$NOW  $ZONE (UTC$OFFSET)"
    echo "Unix 时间戳：$(date '+%s')"
    echo

    # ---- 2. 最近使用的 10 条命令 ----
    echo "[2] 最近使用的 10 条命令"
    echo "--------------------------------------------------"
    # history 只有在交互式 Shell 里才有内存记录；
    # 脚本是非交互式运行的，所以优先读历史文件，同时兼容 bash 与 zsh
    if [[ $- == *i* ]]; then
        # 交互式：直接用 fc 取最后 10 条
        fc -ln -10 2>/dev/null | sed 's/^[[:space:]]*//' | nl -w2 -s'. '
    else
        HIST_SRC=""
        for cand in "${HISTFILE:-}" "$HOME/.bash_history" "$HOME/.zsh_history"; do
            if [ -n "$cand" ] && [ -r "$cand" ]; then
                HIST_SRC="$cand"
                break
            fi
        done

        if [ -n "$HIST_SRC" ]; then
            echo "（来源：${HIST_SRC/#$HOME/~}）"
            # zsh 的历史记录带 ": 时间戳:秒数;" 前缀，先剥掉
            tail -n 10 "$HIST_SRC" | sed 's/^: [0-9]*:[0-9]*;//' | nl -w2 -s'. '
        else
            echo "（当前 Shell 没有可读的历史文件）"
            echo "（history 只记录交互式 Shell 的输入，在终端里正常使用过后即可读到）"
        fi
    fi
    echo

    # ---- 3. 笔记的字数 ----
    echo "[3] 我的笔记的字数"
    echo "--------------------------------------------------"
    if [ -f "$NOTE_FILE" ]; then
        # wc -m 统计字符数（需要 UTF-8 locale，中文才能按「字」计数）
        CHAR_NO_SPACE="$(tr -d '[:space:]' < "$NOTE_FILE" | wc -m | tr -d '[:space:]')"
        CHAR_TOTAL="$(wc -m < "$NOTE_FILE" | tr -d '[:space:]')"
        LINE_TOTAL="$(wc -l < "$NOTE_FILE" | tr -d '[:space:]')"

        echo "笔记文件：notes/$(basename "$NOTE_FILE")"
        echo "字数（不含空白字符）：$CHAR_NO_SPACE"
        echo "字符总数（含空白）　：$CHAR_TOTAL"
        echo "行数　　　　　　　　：$LINE_TOTAL"
    else
        echo "未找到笔记文件（查找目录：$NOTES_DIR）"
        echo "请确认 notes/ 下存在英文命名的 .md 笔记"
    fi
    echo

    echo "=================================================="
    echo " 报告结束"
    echo "=================================================="
}

# ---------- 输出并重定向 ----------
# tee 既打印到终端，又写入同目录下的 output.txt
print_report | tee "$OUT_FILE"

echo "输出已重定向到：$OUT_FILE"
