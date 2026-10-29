#!/usr/bin/env bash
#
# homework.sh -- 第一课作业脚本
#
# 运行后在终端打印三部分信息，并重定向保存到同目录下的 output.txt：
#   1. 运行脚本时的系统时间
#   2. 最近使用的 10 条命令
#   3. notes 目录下笔记的字数统计
#
# 用法：
#   chmod +x homework.sh
#   ./homework.sh
#
# 注意：脚本以子进程方式运行，无法访问当前交互式 Shell 的内存历史，
# 因此“最近 10 条命令”改为从历史文件（$HISTFILE / ~/.bash_history）读取。

set -uo pipefail

# 脚本所在目录（无论从哪个路径调用都能正确定位）
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUTPUT_FILE="${SCRIPT_DIR}/output.txt"

# 笔记目录在 homework 的同级目录 notes 下
NOTES_DIR="$(cd "${SCRIPT_DIR}/../notes" 2>/dev/null && pwd || true)"
NOTES_FILE=""
if [[ -n "${NOTES_DIR}" ]]; then
    NOTES_FILE="$(ls "${NOTES_DIR}"/*.md 2>/dev/null | head -n 1)"
fi

# ---------------------------------------------------------------- 辅助函数

print_last_commands() {
    local hist_file="${HISTFILE:-${HOME}/.bash_history}"

    if [[ -r "${hist_file}" && -s "${hist_file}" ]]; then
        # 历史文件里可能有重复或空行，取最后 10 条非空记录
        grep -v '^[[:space:]]*$' "${hist_file}" | tail -n 10
    elif [[ -n "${ZSH_HISTORY:-}" && -r "${ZSH_HISTORY}" ]]; then
        tail -n 10 "${ZSH_HISTORY}"
    else
        # 兜底：如果是在交互式环境中 source 本脚本，history 仍然可用
        history 10 2>/dev/null || echo "无法读取命令历史（${hist_file} 不存在）"
    fi
}

print_notes_statistics() {
    if [[ -z "${NOTES_FILE}" || ! -f "${NOTES_FILE}" ]]; then
        echo "未找到笔记文件：${NOTES_DIR:-${SCRIPT_DIR}/../notes}/*.md"
        return
    fi

    local lines words chars
    lines="$(wc -l < "${NOTES_FILE}" | tr -d '[:space:]')"
    words="$(wc -w < "${NOTES_FILE}" | tr -d '[:space:]')"
    chars="$(wc -m < "${NOTES_FILE}" | tr -d '[:space:]')"

    echo "笔记文件：${NOTES_FILE}"
    echo "行数：${lines}"
    echo "字数（以空白分词）：${words}"
    echo "字符数（中文按字计）：${chars}"
}

# ---------------------------------------------------------------- 主流程

{
    echo "=============================================="
    echo " homework.sh 运行结果"
    echo "=============================================="
    echo

    echo "[1] 系统时间"
    echo "----------------------------------------------"
    date '+%Y-%m-%d %H:%M:%S %Z (UTC%z)'
    echo

    echo "[2] 最近使用的 10 条命令"
    echo "----------------------------------------------"
    print_last_commands
    echo

    echo "[3] 笔记字数"
    echo "----------------------------------------------"
    print_notes_statistics
    echo
} > "${OUTPUT_FILE}" 2>&1

cat "${OUTPUT_FILE}"
echo
echo "结果已写入：${OUTPUT_FILE}"
