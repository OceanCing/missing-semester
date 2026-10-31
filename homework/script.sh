echo "1.当前系统时间" > output.txt
date >> output.txt
echo "2.最近使用的十条命令" >>output.txt
fc -l -n -10 >> output.txt
echo "3.笔记字数" >> output.txt
wc -m../notes/notes.md >> output.txt
