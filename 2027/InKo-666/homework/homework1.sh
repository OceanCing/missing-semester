{
echo "时间："
date 

echo "最近的10条指令:"
history 10

echo "笔记字数:"
wc -w ../notes/*.md
} > output.txt
