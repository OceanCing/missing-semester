#!/bin/bash
{
echo "系统时间："
date

echo "最近使用的10条命令："
tail -n 10 ~/.bash_history

echo "笔记的字数："
wc -m < ../notes/first_learn.md

}>output.txt
