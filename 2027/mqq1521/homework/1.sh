echo "系统时间" > output.txt
date >> output.txt
history 10 >> output.txt
wc -m ../notes/note.md >> output.txt
