#!/usr/bin/env bash
cd -- "$(dirname -- "$0")" || exit 1

{
    echo "系统时间："
    date

    echo "最近使用的10条命令："
    sed '/^#[0-9][0-9]*$/d; /^[[:space:]]*$/d' "${HISTFILE:-$HOME/.bash_history}" | tail -n 10

    echo "笔记非空白字符数："
    tr -d '[:space:]' < ../notes/lesson-one.md | LC_ALL=C.UTF-8 wc -m
} > output.txt
