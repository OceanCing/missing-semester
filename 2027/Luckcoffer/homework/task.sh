touch output.txt
date >>output.txt
tail -n 10 ~/.bash_history  >> output.txt
wc -w "../notes/first_lesson.md" >>output.txt
