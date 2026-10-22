#!/bin/bash
echo "当前系统时间：$(date)"
echo "最近10条命令："
history | tail -n 10

