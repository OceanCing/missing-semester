#!/usr/bin/env bash
#
# 培训课第一课作业脚本
# 输出：运行时的系统时间、最近使用的 10 条命令、笔记的字数
# 结果统一重定向到与本脚本同目录的 output.txt
#
# 用法: bash homework.sh   或   ./homework.sh

set -uo pipefail

# 固定 UTF-8 locale，保证 wc -m 按字符数、日期格式稳定，不受运行环境影响
export LC_ALL=C.UTF-8 2>/dev/null || export LC_ALL=en_US.UTF-8

# ---- 路径定位：无论从哪个目录调用，都定位到脚本自身所在目录 ----
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NOTES_DIR="$(cd "${SCRIPT_DIR}/../notes" 2>/dev/null && pwd || echo "${SCRIPT_DIR}/../notes")"
OUTPUT_FILE="${SCRIPT_DIR}/output.txt"

# ---- 最近使用的 10 条命令 ----
# 非交互式 shell 没有内存中的 history，此时退回去读 $HISTFILE
get_recent_commands() {
    local lines=""
    local hist_file="${HISTFILE:-${HOME}/.bash_history}"

    # 交互式 shell：直接用内置 history
    if history 1 >/dev/null 2>&1; then
        lines=$(history | tail -n 10)
    fi

    # 非交互式 shell（如脚本被直接调用）：退回读取历史文件
    if [ -z "${lines}" ] && [ -r "${hist_file}" ]; then
        lines=$(tail -n 10 "${hist_file}" | sed 's/^[[:space:]]*[0-9]\{1,\}[[:space:]]*//' | sed '/^$/d')
    fi

    if [ -n "${lines}" ]; then
        echo "${lines}"
    else
        echo "(未找到命令历史：既无交互式 history，也没有可读的历史文件)"
    fi
}

# ---- 笔记字数统计 ----
# 词数 (wc -w) 对英文准确；字符数 (wc -m) 对中文更有参考意义，两者都输出
count_notes() {
    local files
    files=$(find "${NOTES_DIR}" -maxdepth 1 -type f -name '*.md' 2>/dev/null | sort)
    if [ -z "${files}" ]; then
        echo "0 0 0"
        return
    fi

    local words chars files_count
    # shellcheck disable=SC2086
    words=$(cat ${files} | wc -w | tr -d ' ')
    # shellcheck disable=SC2086
    chars=$(cat ${files} | wc -m | tr -d ' ')
    files_count=$(echo "${files}" | wc -l | tr -d ' ')

    echo "${words} ${chars} ${files_count}"
}

main() {
    local read_words read_chars note_files
    read -r read_words read_chars note_files <<< "$(count_notes)"

    {
        echo "=============================================="
        echo " 培训课第一课作业 · output"
        echo "=============================================="
        echo

        echo "----- 1. 运行脚本时的系统时间 -----"
        case "$(date +%u)" in
            1) weekday="星期一" ;;
            2) weekday="星期二" ;;
            3) weekday="星期三" ;;
            4) weekday="星期四" ;;
            5) weekday="星期五" ;;
            6) weekday="星期六" ;;
            *) weekday="星期日" ;;
        esac
        echo "$(date '+%Y-%m-%d %H:%M:%S') ${weekday} (UTC$(date +%z))"
        echo

        echo "----- 2. 最近使用的 10 条命令 -----"
        get_recent_commands
        echo

        echo "----- 3. 笔记的字数 -----"
        echo "笔记目录: ${NOTES_DIR}"
        echo "笔记文件数: ${note_files}"
        echo "词数 (words): ${read_words}"
        echo "字符数 (characters): ${read_chars}"
        echo

        echo "----- 4. 运行环境 -----"
        echo "Shell: ${BASH_VERSION}"
        echo "工作目录: $(pwd)"
        echo "=============================================="
    } > "${OUTPUT_FILE}"

    echo "已生成: ${OUTPUT_FILE}"
}

main "$@"
