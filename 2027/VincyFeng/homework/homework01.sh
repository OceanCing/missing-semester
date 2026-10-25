CURRENT_TIME=$(date +"%Y-%m-%d %H:%M:%S")

echo "Current time: $CURRENT_TIME" > output.txt

echo "Command history top 10:" >> output.txt
history 10 >> output.txt

echo "Note count: $(wc -m < ../notes/NOTE01.md)" >> output.txt
