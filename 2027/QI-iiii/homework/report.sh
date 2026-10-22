
#!/bin/bash
cd ~/missing-semester/2027/QI-iiii/homework

echo "系统时间：" > output.txt
date >> output.txt

echo "最近使用的10条命令：" >> output.txt
tail -n 10 ~/.bash_history >> output.txt

echo "" >> output.txt
echo "笔记字符总数：" >> output.txt

cd ../notes

cat linux/*.md git/*.md nvim/*.md c++*/*/*.md | wc -m >> ../homework/output.txt
