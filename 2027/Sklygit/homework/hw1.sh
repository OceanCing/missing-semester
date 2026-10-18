#!/usr/bin/env bash
#
# 2027/<你的用户名>/homework/hw1.sh
# Missing Semester 培训课 · 第一次作业脚本
#
# 功能：输出运行时刻的系统时间、最近使用的 10 条命令、笔记的字数，
#       并写入（重定向到）与本脚本同目录的 output.txt。
#
# 用法：
#   ./hw1.sh                 # 交互运行：结果写进同目录的 output.txt
#   ./hw1.sh > output.txt    # 或用 shell 重定向（同样支持）
#   ./hw1.sh -o result.txt   # 指定输出文件
#   ./hw1.sh -h              # 查看帮助
#
set -euo pipefail

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
NOTE_DIR="$SCRIPT_DIR/../notes"
DEFAULT_OUT="$SCRIPT_DIR/output.txt"
OUT_FILE="$DEFAULT_OUT"

usage() {
    cat <<'EOF'
用法: ./hw1.sh [-o 输出文件]
  不带参数        结果写入脚本同目录的 output.txt
  > 文件          用 shell 重定向
  -o 文件         指定输出文件
EOF
}

while getopts ":o:h" opt; do
    case $opt in
        o) OUT_FILE=$OPTARG ;;
        h) usage; exit 0 ;;
        *) printf '未知参数：-%s（用 -h 看用法）\n' "$OPTARG" >&2; exit 1 ;;
    esac
done

# 交互式直接运行时（stdout 还是终端），自己把输出重定向到 output.txt；
# 如果调用者已经用了 `> 文件`，就尊重调用者的重定向，避免生成一个空文件。
WROTE_FILE=0
if [ "$OUT_FILE" != "$DEFAULT_OUT" ] || [ -t 1 ]; then
    exec >"$OUT_FILE"
    WROTE_FILE=1
fi

echo "================ Homework 1 ================"
echo

# ---------- 1. 系统时间 ----------
echo "[1] 运行脚本时的系统时间 / system time"
printf '    %s\n' "$(date '+%Y-%m-%d %H:%M:%S %Z (%A)')"
echo

# ---------- 2. 最近使用的 10 条命令 ----------
# 注意：脚本是非交互 shell，`history` 在这里是空列表，所以直接读历史文件。
echo "[2] 最近使用的 10 条命令 / last 10 commands"
HIST_FILE=${HISTFILE:-$HOME/.bash_history}
if [ -r "$HIST_FILE" ]; then
    tail -n 10 "$HIST_FILE" | nl -w 2 -s '. '
else
    echo "    （读不到历史文件：$HIST_FILE）"
fi
echo

# ---------- 3. 笔记字数 ----------
echo "[3] 我的笔记字数 / notes statistics"
NOTE_FILE=$(find "$NOTE_DIR" -maxdepth 1 -type f -name '*.md' -print 2>/dev/null | sort | head -n 1 || true)
if [ -n "${NOTE_FILE:-}" ]; then
    words=$(wc -w <"$NOTE_FILE" | tr -d ' ')
    chars=$(wc -m <"$NOTE_FILE" | tr -d ' ')
    lines=$(wc -l <"$NOTE_FILE" | tr -d ' ')
    printf '    笔记文件 / file : %s\n' "${NOTE_FILE#"$SCRIPT_DIR"/../}"
    printf '    单词数 / words  : %s\n' "$words"
    printf '    字符数 / chars  : %s\n' "$chars"
    printf '    行  数 / lines  : %s\n' "$lines"
else
    printf '    在 %s 下没找到 .md 笔记文件\n' "$NOTE_DIR"
fi
echo
echo "============================================"

# 提示信息走 stderr，不污染写进 output.txt 的内容
if [ "$WROTE_FILE" -eq 1 ]; then
    printf '结果已写入：%s\n' "$OUT_FILE" >&2
fi
