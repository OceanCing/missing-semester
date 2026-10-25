echo "=== 系统时间 ==="
date
echo ""

echo "=== 最近10条命令 ==="
history 10
echo ""

echo "=== 笔记字数 ==="
NOTES_FILE="../notes/training-notes.md"
if [ -f "$NOTES_FILE" ]; then
    wc -m < "$NOTES_FILE" | xargs echo "笔记字符数："
else
    echo "笔记文件未找到，请检查路径。"
fi
